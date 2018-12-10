# repost_detector_bot
Checks if image has already been posted in a groupme chat

Wrote image comparison functionality in C++, use dlib
libraries for reading in of various image files and 2darray
functionality. 

Built class in which hash values are contained, implemented
by averaging pixel values of sections of images. Comparison
is implemented by comparing the values from sections, with
slight threshold for tolerance to deal with image compression.

Currently appears to be around correct sensitivity, tolerat-
ing smaller deviations that occur throughout the whole image,
but being sensitive to large changes over a small area.

To do:
-Design storage architecture of data for quick comparison. 
-Figure out how to connect run with callback url 
