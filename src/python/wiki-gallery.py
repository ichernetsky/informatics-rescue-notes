#!/usr/bin/env python
# -*- coding: utf-8 -*-

import argparse
import logging
import lxml.etree
import os.path
import Queue
import threading
import urllib
import urllib2
import urlparse

class Thread(threading.Thread):
    def __init__(self, tasks):
        super(Thread, self).__init__()
        self.tasks = tasks
        self.daemon = True

    def run(self):
        logger.debug('%s has been started', self.name)
        while True:
            fn = self.tasks.get()
            try:
                fn()
            except Exception, e:
                logger.debug('%s got exception', self.name, exc_info=e)
            self.tasks.task_done()

class ThreadPool(object):
    def __init__(self, num_threads):
        logger.debug('Initializing ThreadPool with %u workers', num_threads)
        self.tasks = Queue.Queue()
        self.threads = [Thread(self.tasks) for i in xrange(num_threads)]
        logger.debug('ThreadPool has been initialized')

    def start(self):
        for thread in self.threads:
            thread.start()

    def add(self, task):
        self.tasks.put(task)
        logger.debug('%s has been put in queue', task)

    def join(self):
        self.tasks.join()
        logger.debug('All threads are stopped')

class UrlHandler(object):
    def __init__(self, url):
        self.url = url

    def __str__(self):
        return '%s for %s' % (self.__class__.__name__, self.url)

    @staticmethod
    def download(url):
        request = urllib2.Request(url=url)
        request.add_header('User-agent', 'Mozilla/5.0')
        return urllib2.urlopen(request)

    @staticmethod
    def make_tree(page):
        parser = lxml.etree.HTMLParser()
        tree = lxml.etree.parse(page, parser)
        return tree

class CategoryHandler(UrlHandler):
    def __init__(self, url, pool):
        super(CategoryHandler, self).__init__(url)
        self.pool = pool

    def __call__(self):
        page = self.download(self.url)

        file_pages = self.parse(page)
        for u in file_pages:
            handler = FileHandler(u)
            self.pool.add(handler)

    def parse(self, page):
        tree = self.make_tree(page)
        return [urlparse.urljoin(self.url, url)
                for url in tree.xpath('//li[@class="gallerybox"]//a[@class="image"]/@href')]

class FileHandler(UrlHandler):
    def __call__(self):
        page = self.download(self.url)

        files = self.parse(page)
        for u in files:
            fname = self.file_name(u)
            if os.path.exists(fname):
                continue

            remotef = self.download(u)
            f = file(fname, "w")
            f.write(remotef.read())
            logger.info('%s downloaded', u)

    def parse(self, page):
        tree = self.make_tree(page)
        return [urlparse.urljoin(self.url, url)
                for url in tree.xpath('//div[@class="fullImageLink"]/a/@href')]

    @staticmethod
    def file_name(url):
        fname = url.rsplit('/', 1)[1]
        fname = urllib.unquote_plus(fname)
        return fname

logger = None

def setup_logger(level):
    global logger
    logger = logging.getLogger('wiki-gallery')
    logger.setLevel(getattr(logging, level.upper()))
    console = logging.StreamHandler()
    console.setLevel(getattr(logging, level.upper()))
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    console.setFormatter(formatter)
    logger.addHandler(console)

def main():
    parser = argparse.ArgumentParser(description='Download images from wiki categories.')
    parser.add_argument('urls', metavar='url', type=unicode, nargs='+',
                        help='urls for wiki categories')
    parser.add_argument('--log-level', dest='log_level', action='store',
                        choices=['debug', 'info', 'warning', 'error', 'critical'],
                        default='warning', help='log level')
    args = parser.parse_args()

    setup_logger(args.log_level)

    pool = ThreadPool(20)

    for url in args.urls:
        page_handler = CategoryHandler(url, pool)
        pool.add(page_handler)

    pool.start()
    pool.join()

if __name__ == '__main__':
    main()
