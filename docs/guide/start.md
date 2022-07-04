Getting Started with Fitcrack
=============================

Welcome to Fitcrack – The distributed password cracking system. This is your comprehensive and friendly user's guide. It focuses on the Webadmin app, which is the primary way of controlling the Fitcrack system. It was designed to be elegant and as user-friendly as possible for a password cracking control panel. Here's what you can expect to find in this documentaion:

- Getting familliar with the interface
- An overview of how Fitcrack works
- How to connect compute nodes
- How to create a cracking job
- What options are available for cracking
- How to work with jobs
- How to extend the system asset library
- How to manage users and resources

Before we can get started with all of that, however, you will want to have Fitcrack installed and ready to go. If you don't know what to do, please refer to the [installation guide](//github.com/nesfit/fitcrack/blob/master/README.md). After setting up, come back and we can get started.


Preflight Checks
----------------

After installing Fitcrack, you should have the system ready to use on your server. It is the main control center, housing the central database, communicating with all of the connected host nodes, and, crucially for us, running the Webadmin suite.

Now, depending on how you set the system up and whether you are exposing the web server to the internet, there will be different ways of connecting to Webadmin. In any case, you should be able to launch the app by visiting your server's domain via a web browser. When you do, you should be greeted by a login screen, just like below.

![Fitcrack Login](../_media/img/overview/login.png)


First login
-----------

After a fresh install of Fitcrack, there is a default administrator user for Webadmin ready to go. To log in, use the following credentials: Username `fitcrack` with password `FITCRACK`.

It is a good idea to change the password after your first login. To do this, visit the _My Account_ page from the main navigation bar on the left and use the _Change Password_ box.

And that's it, you have successfully logged into Webadmin and are now ready to start using Fitcrack! Let's go take a look around the app next.
