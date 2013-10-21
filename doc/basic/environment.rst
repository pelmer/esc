School exercise environment
===========================

The goal of this step is to prepare your working environment for the ESC
exercises.  At the end of this step you should have a reasonably workable
environment for running the other exercises.

You will work using the account created for you. In what follows we will
write "<student>" to indicate where you should substitute the username you
were assigned.
  * You were assigned a host between esc-vm-01 and -52 to work on. Two 
    students share each server.
  * Your home directory is ``/storage/nfs_esc13/<student>`` and is local 
    to the server, where "<student>" is your assigned login name
  * Your shell is `bash <http://www.gnu.org/s/bash/>`_
  * Your NFS area is in ``/storage/nfs_esc13/<student>``

Below we use *<student>* as a general student account "esc-server-<nn>" as the
general host alias.  When executing the commands, replace these with the
account name or host assigned to you.  **IMPORTANT NOTE:** *The student
computers will be uninstalled on Friday evening and all data on them will be
destroyed. Please make sure you make a copy of everything valuable by the
end of the Friday session!*

Simple configuration
--------------------

This is the minimal basic configuration you should follow at the very least.
If you are working on Mac OS X or Linux laptop and moderately comfortable
with SSH and would rather you use the editor on your local system, the more
advanced steps below give an alternate extended configuration which makes
working somewhat nicer and easier.

0. Make sure you are using the INFN-Web WiFi network and not the CEUB
   WiFi network. Only the INFN-Web one will work for the following. If
   you do not have a user/pass to connecto the the INFN-Web network,
   ask one of the organizers.

1. First SSH into the gateway NSF server:
     ssh <student>@131.154.193.30

2. You can only connect out to github from this gateway server, but it
   has the same home area. Get the school exercises material::

     git clone git://github.com/pelmer/esc.git

   (If you need to update this during the school, you will have to do it
    from this gateway server, not the assigned host.)

3. Then from there SSH into your assigned host::

     ssh <student>@esc-vm-<nn>
     echo $HOME $SHELL
     ls -laF

Advanced configuration
----------------------

The assumption for this more advanced section is that you prefer to work on
your laptop, editing exercise code there. In parallel you will keep several
terminal windows open on the student server, where you compile code and run
other commands. We set up SSH keys so you can login to your server without
constantly typing your password, plus some convenient name aliases so you do
need to remember IP addresses.

1. SSH into your assigned host::

     ssh <student>@esc-vm-<nn>
     echo $HOME $SHELL
     mkdir -p .ssh
     chmod 755 .ssh
     ls -laF

2. If your laptop is Linux, Mac OS X, or if you use Cygwin on Windows, we
   recommend you keep a copy of all your exercise source code, notes etc.
   locally, and `rsync <http://rsync.samba.org/>`_ to/from student servers.
   This ensures you keep a copy of everything relevant locally at the end.

   * Generate a temporary SSH key and copy it to the right host::

       ssh-keygen -t rsa -C esc13_temp_key -f ~/.ssh/id_rsa_esc13
       scp ~/.ssh/id_rsa_esc13.pub <student>@131.154.193.30:.ssh/authorized_keys

   * If not on OS X, start an ``ssh-agent``::

       [ $(uname) != Darwin ] && { killall ssh-agent; eval `ssh-agent`; }

   * Add the just-generated key to your ``ssh-agent``::

       ssh-add ~/.ssh/id_rsa_esc13

   * Test you can rsync to the student host (change "esc-40" below to your
     assigned node!)::

       mkdir /tmp/foo
       date > /tmp/foo/test.txt
       rsync -av /tmp/foo/ 131.154.193.30:foo/
       ssh <student>@131.154.193.30 ls -laFR foo
       ssh <student>@131.154.193.30 rm -fr foo
       rm -fr /tmp/foo

3. Get the school exercises material on your laptop::

     cd My/Dev/Area
     git clone https://github.com/pelmer/esc.git

     # If you get SSL error, 'export GIT_SSL_NO_VERIFY=true' first.
     # The local firewall prevents use of 'git:' url style on your laptop.

4. Synchronise to your student server::

     rsync -av esc/ 131.154.193.30:esc/

   You can download a `zip file <https://github.com/pelmer/esc/zipball/master>`_
   or `tar ball <https://github.com/pelmer/esc/tarball/master>`_ from the
   `github web page <http://github.com/pelmer/esc>`_ if you don't have ``git``.

   You can now edit sources on your laptop, and run the command above to sync
   to the server. You can of course also run an editor on the server and sync
   back to your laptop, which ever you feel more comfortable with. Do be
   careful with rsync command syntax, however, it's easy to make mistakes.

Finishing off
-------------

1. Open one or more terminal windows and ssh into the student server, and
   in each of them run the following environment setup::

     alias c++=c++-482
     alias g++=g++-482
     alias gcc=gcc-482
     export PATH=/export/software/elmer/igprof/bin:/export/software/elmer/valgrind/bin:$PATH
     export LD_LIBRARY_PATH=/export/software/elmer/igprof/lib:/export/software/elmer/valgrind/lib

2. Check the following are working ok::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.8.2 (GCC)'
     valgrind --version          # should say 'valgrind-3.8.1'
     igprof -h                   # should print simple help message
     which igprof-navigator      # should say full path

3. Create a web area where you will put output from some exercises::

     mkdir -p /storage/nfs_esc13/<student>/public_html/cgi-bin/data
     cp $(which igprof-navigator) ~/public_html/cgi-bin/igprof-navigator.py
     echo "<html><body><a href='cgi-bin/igprof-navigator.py'>My" \
       "igprof reports</a></body></html>" > ~/public_html/index.html
     chmod 755 ~/public_html/cgi-bin

4. View http://131.154.193.30/~<student>/ in your web browser, you should see
   the basic page you created above. Click on the link to profiles, it should
   produce some output -- for now it will in fact display an error message
   due to lack of profiles.

5. You should be able to view some of these CMS profile examples:
   `perf report <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias02_perf/>`_,
   `heap snapshot after 50 events <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03.50_live/>`_,
   `total dynamic memory allocations <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03_total/>`_.

In subsequent exercises, you should have one more terminal windows open with
ssh session to the student server, and source the environment setup script as
shown below. **Always start each new exercise in a fresh new shell 
environment!**

If you are adventurous, you can build igprof standalone yourself following
the `recipe on the web site <http://igprof.sourceforge.net/install.html>`_.
