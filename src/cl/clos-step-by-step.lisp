(defclass bank-account () ())

(defclass bank-account ()
  (customer-name
   balance))

(defparameter *account* (make-instance 'bank-account))

(setf (slot-value *account* 'customer-name) "John Doe")
(setf (slot-value *account* 'balance) 1000)

(slot-value *account* 'customer-name)
(slot-value *account* 'balance)

(defclass bank-account ()
  ((customer-name :initarg :customer-name)
   (balance :initarg :balance
            :initform 0)))

(defparameter *account*
  (make-instance 'bank-account
                 :customer-name "John Doe"
                 :balance 1000))

(slot-value (make-instance 'bank-account) 'balance)
(slot-value (make-instance 'bank-account) 'customer-name)

(defvar *account-numbers* 0)

(defclass bank-account ()
  ((customer-name :initarg :customer-name
                  :initform (error "Must supply a customer name."))
   (balance :initarg :balance
            :initform 0)
   (account-number :initform (incf *account-numbers*))))

(defclass bank-account ()
  ((customer-name
    :initarg :customer-name
    :initform (error "Must supply a customer name.")
    :accessor customer-name
    :documentation "Customer's name")
   (balance
    :initarg :balance
    :initform 0
    :reader balance
    :documentation "Current account balance")
   (account-number
    :initform (incf *account-numbers*)
    :reader account-number
    :documentation "Account number, unique within a bank.")))

(defgeneric withdraw (account amount)
  (:documentation "Withdraw the specified amount from the account.
Signal an error if the current balance is less than amount."))

(defmethod withdraw ((account bank-account) amount)
  (when (< (balance account) amount)
    (error "Account overdrawn."))
  (decf (slot-value account 'balance) amount))

(defclass checking-account (bank-account)
  ((overdraft-account
    :initarg :overdraft-account
    :initform (error "Must supply an overdraft account.")
    :reader overdraft-account)))

(defmethod withdraw ((account checking-account) amount)
  (let ((overdraft (- amount (balance account))))
    (when (plusp overdraft)
      (withdraw (overdraft-account account) overdraft)
      (incf (slot-value account 'balance) overdraft)))
  (call-next-method))

(defvar *account-of-bank-president*
  (make-instance 'bank-account
                 :customer-name "Mr. President"
                 :balance 100000000000))

(defclass bank () ())
(defvar *bank* (make-instance 'bank))

(defun embezzle (bank overdraft)
  (declare (ignore bank))
  overdraft)

(defmethod withdraw ((account (eql *account-of-bank-president*)) amount)
  (let ((overdraft (- amount (balance account))))
    (when (plusp overdraft)
      (incf (slot-value account 'balance) (embezzle *bank* overdraft)))
    (call-next-method)))

(defmethod withdraw :before ((account checking-account) amount)
  (let ((overdraft (- amount (balance account))))
    (when (plusp overdraft)
      (withdraw (overdraft-account account) overdraft)
      (incf (slot-value account 'balance) overdraft))))

(defclass bank-account ()
  ((customer-name
    :initarg :customer-name
    :initform (error "Must supply a customer name.")
    :accessor customer-name
    :documentation "Customer's name")
   (balance
    :initarg :balance
    :initform 0
    :reader balance
    :documentation "Current account balance")
   (account-number
    :initform (incf *account-numbers*)
    :reader account-number
    :documentation "Account number, unique within a bank.")
   (account-type
    :reader account-type
    :documentation "Type of account, one of :gold, :silver, or :bronze.")))

(defmethod initialize-instance :after ((account bank-account) &key)
  (let ((balance (slot-value account 'balance)))
    (setf (slot-value account 'account-type)
          (cond
            ((>= balance 100000) :gold)
            ((>= balance 50000) :silver)
            (t :bronze)))))

(defclass job () ())
(defclass express-job (job) ())

(defgeneric priority (job)
  (:documentation "Return the priority at which the job should be run.")
  (:method-combination +))

(defgeneric priority (job)
  (:documentation "Return the priority at which the job should be run.")
  (:method-combination + :most-specific-last))

(defmethod priority + ((job express-job)) 10)

(defclass asteroid () ())
(defclass spaceship () ())

(defgeneric collide (x y)
  (:documentation
   "Do necessary calculations on collision of objects."))

(defmethod collide ((x asteroid) (y spaceship))
  (format t "Collide: asteroid and spaceship."))

(defmethod collide ((x spaceship) (y asteroid))
  (collide y x))

(defmethod collide ((x asteroid) (y asteroid))
  (format t "Collide: two asteroids."))

(defmethod collide ((x spaceship) (y spaceship))
  (format t "Collide: two spaceships"))
