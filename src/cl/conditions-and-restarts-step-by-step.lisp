(defclass log-entry ()
  ((text :initarg :text :accessor text)))

(define-condition malformed-log-entry-error (error)
  ((text :initarg :text :reader text))
  (:report (lambda (condition stream)
             (format stream "Malformed entry ~S occured." (text condition)))))

(defun well-formed-log-entry-p (text)
  (when (not (zerop (length text)))
    (every #'(lambda (c) (char= c #\a)) text)))

(defun parse-log-entry (text)
  (if (well-formed-log-entry-p text)
      (make-instance 'log-entry :text text)
      (error 'malformed-log-entry-error :text text)))
      
(defun parse-log-file (file)
  (with-open-file (in file :direction :input)
    (loop
       for text = (read-line in nil nil)
       while text
       for entry = (handler-case (parse-log-entry text)
                     (malformed-log-entry-error () nil))
       when entry
         collect it)))

(defun analyze-entry (entry)
  (length (text entry)))

(defun analyze-log (log)
  (dolist (entry (parse-log-file log))
    (format t "~a: ~a~%" (text entry) (analyze-entry entry))))

(defun parse-log-file (file)
  (with-open-file (in file :direction :input)
    (loop
       for text = (read-line in nil nil)
       while text
       for entry = (restart-case (parse-log-entry text)
                     (skip-log-entry ()
                       :report "Skip the malformed entry."
                       nil))
       when entry
         collect it)))

(defun analyze-log (log)
  (handler-bind ((malformed-log-entry-error
                  #'(lambda (c)
                      (declare (ignore c))
                      (invoke-restart 'skip-log-entry))))
    (dolist (entry (parse-log-file log))
      (format t "~a: ~a~%" (text entry) (analyze-entry entry)))))

(defun skip-log-entry (c)
  (declare (ignore c))
  (invoke-restart 'skip-log-entry))

(defun analyze-log (log)
  (handler-bind ((malformed-log-entry-error #'skip-log-entry))
    (dolist (entry (parse-log-file log))
      (format t "~a: ~a~%" (text entry) (analyze-entry entry)))))

(defun skip-log-entry (c)
  (declare (ignore c))
  (let ((restart (find-restart 'skip-log-entry)))
    (when restart
      (invoke-restart restart))))

(defun parse-log-entry (text)
  (if (well-formed-log-entry-p text)
      (make-instance 'log-entry :text text)
      (restart-case (error 'malformed-log-entry-error :text text)
        (use-value (value)
          value)
        (reparse-entry (fixed-text)
          (parse-log-entry fixed-text)))))

(defun analyze-log (log)
  (dolist (entry (parse-log-file log))
    (format t "~a: ~a~%" (text entry) (analyze-entry entry))))

(defun parse-log-entry (text)
  (if (well-formed-log-entry-p text)
      (make-instance 'log-entry :text text)
      (restart-case (error 'malformed-log-entry-error :text text)
        (use-value (value)
          :report "Use provided value."
          :interactive (lambda ()
                         (format t "Enter a new value: ")
                         (list (make-instance 'log-entry :text (read-line))))
          value)
        (reparse-entry (fixed-text)
          :report "Provide text to be parsed instead."
          :interactive (lambda ()
                         (format t "Enter text to be parsed: ")
                         (list (read-line)))
          (parse-log-entry fixed-text)))))

(defun analyze-log (log)
  (handler-bind ((malformed-log-entry-error
                  #'(lambda (c)
                      (format t "~A~%" c)
                      (if (y-or-n-p "Skip this entry")
                          (invoke-restart 'skip-log-entry)
                          (progn
                            (format t "Please provided a new value to use instead: ")
                            (invoke-restart 'reparse-entry (read-line)))))))
    (dolist (entry (parse-log-file log))
      (format t "~a: ~a~%" (text entry) (analyze-entry entry)))))
