Controlling Jobs
================

During the lifecycle of a job in the system, it goes through different states. Depending on the current state, the job can be controlled in different ways.

When a job is created, it starts off in a ready state, not doing anything yet. It can be started or modified, and later in the lifecycle, it can be stopped, restarted or, should the need arise, purged.


Starting and Restarting
-----------------------

Jobs that are ready can be started. Jobs that are no longer running, be it after successful finish or in an error state, can be restarted and will run again.

Before a job can start, it has to have hosts assigned. If it doesn't, you will see a yellow warning and a button to assign the hosts instead of the typical job controls.


Stopping and Purging
--------------------

You can stop a job prematurely or purge (kill) it if there are problems with it. As an example, some jobs may grind to a halt and get stuck in a finishing state. Purging such job will stop all participating hosts from working on it and will return the job to a clean ready state.


Reassigning Hosts
-----------------

You may want to assign new hosts to a job or remove already assigned ones. This can be helpful when you want to add hosts that joined the system after the job was created, for example.

If you want to change the host assignment, you can do so from the Hosts panel in the detail view. The host assignment dialog updates automatically.
