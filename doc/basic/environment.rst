School exercise environment
===========================

The goal of this step is to prepare your working environment for the ESC
exercises.  At the end of this step you should have a reasonably workable
environment for running the other exercises.

You will work using the account created for you. In what follows we will
write "<student>" to indicate where you should substitute the username you
were assigned.
  * You were assigned a host between 137.204.203.40 -- .47 to work on. Two 
    students share each server.
  * Your home directory is ``/home/<student>`` and is local to the server.
  * Your shell is `bash <http://www.gnu.org/s/bash/>`_
  * Your NFS area is in ``/storage/nfs_esc12/<student>``

Below we use *<student>* as a general student account and 137.204.203.40 as the
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

1. SSH into your assigned host::

     ssh <student>@137.204.203.40
     echo $HOME $SHELL
     ls -laF

2. Get the school exercises material::

     git clone git://github.com/pelmer/esc.git

Advanced configuration
----------------------

The assumption for this more advanced section is that you prefer to work on
your laptop, editing exercise code there. In parallel you will keep several
terminal windows open on the student server, where you compile code and run
other commands. We set up SSH keys so you can login to your server without
constantly typing your password, plus some convenient name aliases so you do
need to remember IP addresses.

1. SSH into your assigned host::

     ssh <student>@137.204.203.40
     echo $HOME $SHELL
     mkdir -p .ssh
     chmod 755 .ssh
     ls -laF

2. If your laptop is Linux, Mac OS X, or if you use Cygwin on Windows, we
   recommend you keep a copy of all your exercise source code, notes etc.
   locally, and `rsync <http://rsync.samba.org/>`_ to/from student servers.
   This ensures you keep a copy of everything relevant locally at the end.

   * Generate a temporary SSH key and copy it to the right host::

       ssh-keygen -t rsa -C esc12_temp_key -f ~/.ssh/id_rsa_esc12
       scp ~/.ssh/id_rsa_esc12.pub <student>@137.204.203.40:.ssh/authorized_keys

   * If not on OS X, start an ``ssh-agent``::

       [ $(uname) != Darwin ] && { killall ssh-agent; eval `ssh-agent`; }

   * Add the just-generated key to your ``ssh-agent``::

       ssh-add ~/.ssh/id_rsa_esc12

   * If you want to avoid typing the full IP address to log on to the
     machine provided by the school, you can add some SSH configuration for
     your student host on your laptop.  You can of course add
     this chunk with your favourite editor if you prefer. (Change "esc-40"
     and the IP address to correspond to the machine you were assigned.)

     ::

       cat >> ~/.ssh/config << \EOF
         Host esc-40
           HostName 137.204.203.40
           PubkeyAuthentication yes
           IdentityFile ~/.ssh/id_rsa_esc12
       EOF
       chmod 600 ~/.ssh/config

   * Test you can ssh to your student account without password prompts::

       ssh esc-40 uptime

   * Test you can rsync to the student host (change "esc-40" below to your
     assigned node!):

       mkdir /tmp/foo
       date > /tmp/foo/test.txt
       rsync -av /tmp/foo/ esc-40:foo/
       ssh esc-40 ls -laFR foo
       ssh esc-40 rm -fr foo
       rm -fr /tmp/foo

3. Get the school exercises material on your laptop::

     cd My/Dev/Area
     git clone https://github.com/pelmer/esc.git

     # If you get SSL error, 'export GIT_SSL_NO_VERIFY=true' first.
     # The local firewall prevents use of 'git:' url style on your laptop.

4. Synchronise to your student server::

     rsync -av esc/ esc-40:esc/

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

     . /storage/software/env-gcc472.sh

2. Check the following are working ok::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.7.2 (GCC)'
     valgrind --version          # should say 'valgrind-3.8.0'
     igprof -h                   # should print simple help message
     which igprof-navigator      # should say full path

3. Create a web area where you will put output from some exercises::

     mkdir -p /storage/nfs_esc12/<student>/public_html/cgi-bin/data
     ln -s /storage/nfs_esc12/<student>/public_html ~/public_html
     cp $(which igprof-navigator) ~/public_html/cgi-bin/igprof-navigator.py
     echo "<html><body><a href='cgi-bin/igprof-navigator.py'>My" \
       "igprof reports</a></body></html>" > ~/public_html/index.html
     chmod 755 ~/public_html/cgi-bin

4. View http://137.204.203.61/~<student>/ in your web browser, you should see
   the basic page you created above. Click on the link to profiles, it should
   produce some output -- for now it will in fact display an error message
   due to lack of profiles.

5. You should be able to view some of these CMS profile examples:
   `perf report <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias02_perf/>`_,
   `heap snapshot after 50 events <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03.50_live/>`_,
   `total dynamic memory allocations <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03_total/>`_.

In subsequent exercises, you should have one more terminal windows open with
ssh session to the student server, and source the environment setup script as
shown below.  You will normally use ``env-gcc472.sh``, but in some cases you
will also use ``env-gcc462.sh``. **Always start each new exercise in a fresh
new shell environment!**

If you are adventurous, you can build igprof standalone yourself following
the `recipe on the web site <http://igprof.sourceforge.net/install.html>`_.
