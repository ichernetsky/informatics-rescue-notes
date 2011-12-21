#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import logging
import lxml.etree
import os.path
import Queue
import subprocess
import threading
import urllib
import urllib2
import urlparse

class Worker(threading.Thread):
    def __init__(self, pool):
        super(Worker, self).__init__()

        self.pool   = pool
        self.daemon = True

    def run(self):
        logging.debug('%s has been started', self.name)
        while True:
            fn = self.pool.next_task()
            try:
                fn(self.pool)
            except Exception, e:
                logging.error('%s got exception', self.name, exc_info=e)
            self.pool.task_done()

class WorkerPool(object):
    def __init__(self, num_workers):
        logging.debug('Initializing WorkerPool with %u workers', num_workers)

        self.tasks   = Queue.Queue()
        self.workers = [Worker(self) for i in xrange(num_workers)]

        logging.debug('WorkerPool has been initialized')

    def start(self):
        for worker in self.workers:
            worker.start()
        logging.debug('All workers have been started')

    def add(self, task):
        self.tasks.put(task)

    def next_task(self):
        return self.tasks.get()

    def task_done(self):
        self.tasks.task_done()

    def join(self):
        self.tasks.join()
        logging.debug('All workers are stopped')

class UrlHandler(object):
    def __init__(self, url):
        self.url = url

    def __repr__(self):
        return self.__str__()

    def __str__(self):
        return '%s for %s' % (self.__class__.__name__, self.url)

    @staticmethod
    def download(url):
        request = urllib2.Request(url=url)
        request.add_header('Accept-Charset', 'UTF-8,*;q=0.5')
        request.add_header('Accept-Language', 'en-US,en;q=0.8')
        request.add_header('User-agent', 'Mozilla/5.0')
        return urllib2.urlopen(request)

    @staticmethod
    def make_tree(page):
        parser = lxml.etree.HTMLParser()
        tree = lxml.etree.parse(page, parser)
        return tree

class CategoryHandler(UrlHandler):
    def __init__(self, url):
        super(CategoryHandler, self).__init__(url)

    def __call__(self, pool):
        page = self.download(self.url)

        categories, file_pages = self.parse(page)

        for u in categories:
            handler = CategoryHandler(u)
            pool.add(handler)

        for u in file_pages:
            handler = FileHandler(u)
            pool.add(handler)

    def parse(self, page):
        tree = self.make_tree(page)

        categories = tree.xpath('//div[@id="mw-category-media"]/a')
        categories = [c.attrib['href'] for c in categories
                      if unicode(c.text).strip().lower() == u'next 200']
        categories = [urlparse.urljoin(self.url, c)
                      for c in categories]
        categories = list(set(categories))

        file_pages = [urlparse.urljoin(self.url, url)
                      for url in tree.xpath('//li[@class="gallerybox"]//a[@class="image"]/@href')]

        return categories, file_pages

class FileHandler(UrlHandler):
    def __call__(self, pool):
        page = self.download(self.url)

        description, files = self.parse(page)
        for u in files:
            fname = self.file_name(u)
            if os.path.exists(fname):
                logging.warning('%s exists, skipping it', fname)
                continue

            remotef = self.download(u)
            localf = file(fname, "w")
            localf.write(remotef.read())

            if description:
                try:
                    ret = subprocess.call([u'exiv2',
                                           u'-Mset Exif.Photo.UserComment %s' % description,
                                           fname])
                    logging.debug('exiv2 returned %u', ret)
                except:
                    pass

            logging.info('File %s downloaded', u)

    def parse(self, page):
        tree = self.make_tree(page)

        description = list()
        for text in tree.xpath('//span[@id="creator"]/a/text()'):
            description.append(text)
        for elem in tree.xpath('//div[contains(@class,"description")]'):
            description.append(u' '.join(e.text for e in elem.iter()
                                         if e.text))

        description = u' — '.join(d.strip() for d in description)

        files = [urlparse.urljoin(self.url, url)
                 for url in tree.xpath('//div[@class="fullImageLink"]/a/@href')]

        return description, files

    @staticmethod
    def file_name(url):
        fname = url.rsplit('/', 1)[1]
        fname = urllib.unquote_plus(fname)
        return fname

def setup_logging(level):
    level = getattr(logging, level.upper())
    logging.basicConfig(format='%(asctime)s - %(name)s - %(levelname)s - %(message)s',
                        level=level)

def main():
    parser = argparse.ArgumentParser(description='Download images from wiki categories.')
    parser.add_argument('urls', metavar='url', type=unicode, nargs='+',
                        help='urls for wiki categories')
    parser.add_argument('--log-level', dest='log_level', action='store',
                        choices=['debug', 'info', 'warning', 'error', 'critical'],
                        default='warning', help='log level')
    args = parser.parse_args()

    setup_logging(args.log_level)

    pool = WorkerPool(20)

    for url in args.urls:
        page_handler = CategoryHandler(url)
        pool.add(page_handler)

    pool.start()
    pool.join()

if __name__ == '__main__':
    main()
