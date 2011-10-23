School exercise environment
===========================

The goal of this step is to prepare your working environment for the ESC
exercises.  At the end of this step you should have a reasonably workable
environment for running the other exercises.

1. You will work using *studNN* account created for you.
    * You were assigned host 137.204.203.40 -- .51 to work on. Two students
      share each server.
    * Your home directory is ``/home/studNN``
    * Your shell is `bash <http://www.gnu.org/s/bash/>`_
    * Your NFS area is in ``/storage/nfs_esc11/studNN``
    * Your GPFS area is in ``/storage/gpfs_esc11/studNN``

   Below we use *stud01* as a general student account and 137.204.203.40 as
   the general host alias.  When executing the commands, replace these with
   the account number or host assigned to you.  SSH into your assigned host
   now::

      ssh stud01@137.204.203.40
      echo $HOME $SHELL
      mkdir -p .ssh
      ls -laF

   **IMPORTANT NOTE:** *The student computers will be uninstalled on Friday
   evening and all data on them will be destroyed. Please make sure you
   make a copy of everything valuable by the end of the Friday session!*

2. If your laptop is Linux, Mac OS X, or if you use Cygwin on Windows, we
   recommend you keep a copy of all your exercise source code, notes etc.
   locally, and `rsync <http://rsync.samba.org/>`_ to/from student servers.
   This ensures you keep a copy of everything relevant locally at the end.

   * Generate a temporary SSH key and copy it to the right host::

       ssh-keygen -t rsa -C esc11_temp_key -f ~/.ssh/id_rsa_esc11
       scp ~/.ssh/id_rsa_esc11.pub 137.204.203.40:.ssh/authorized_keys

   * If not on OS X, start an ``ssh-agent``::

       [ $(uname) != Darwin ] && { killall ssh-agent; eval `ssh-agent`; }

   * Add the just-generated key to your ``ssh-agent``::

       ssh-add ~/.ssh/id_rsa_esc11

   * Add SSH configuration for your student host.  Use the appropriate
     student account and IP address below.  You can of course add this
     chunk with your favourite editor if you prefer.

     ::

       cat >> ~/.ssh/config << \EOF
         Host esc-*
           User stud01
           PubkeyAuthentication yes
           IdentityFile ~/.ssh/id_rsa_esc11

         Host esc-stud01
           HostName 137.204.203.40
         Host esc-stud02
           HostName 137.204.203.41
         Host esc-stud03
           HostName 137.204.203.42
         Host esc-stud04
           HostName 137.204.203.43
         Host esc-stud05
           HostName 137.204.203.44
         Host esc-stud06
           HostName 137.204.203.45
         Host esc-stud07
           HostName 137.204.203.46
         Host esc-stud08
           HostName 137.204.203.47
         Host esc-stud09
           HostName 137.204.203.48
         Host esc-stud10
           HostName 137.204.203.49
         Host esc-stud11
           HostName 137.204.203.50
         Host esc-stud12
           HostName 137.204.203.51
         Host esc-adm01
           HostName 137.204.203.67
         Host esc-adm02
           HostName 137.204.203.68
         Host esc-adm03
           HostName 137.204.203.69
         Host esc-adm04
           HostName 137.204.203.70
       EOF
       chmod 600 ~/.ssh/config

   * Test you can ssh to your student account without password prompts::

       ssh esc-stud01 uptime

   * Test you can rsync to the student host::

       mkdir /tmp/foo
       date > /tmp/foo/test.txt
       rsync -av /tmp/foo/ esc-stud01:foo/
       ssh esc-stud01 ls -laFR foo
       ssh esc-stud01 rm -fr foo
       rm -fr /tmp/foo

3. Get the school exercises material. If you did the previous step, then
   do this on your laptop and ``rsync -av esc/ esc-stud01:esc/`` to your
   student account, otherwise do this on the student server in your home
   directory::

     git clone git://github.com/lat/esc.git

   You can download a `zip file <https://github.com/lat/esc/zipball/master>`_
   or `tar ball <https://github.com/lat/esc/tarball/master>`_ from the
   `github web page <http://github.com/lat/esc>`_ if you don't have ``git``.

4. Open one or more terminal windows and ssh into the student server, and
   in each of them run the following environment setup::

     . /storage/software/tuura/env-gcc461.sh

5. Check the following are working ok::

     c++ -v 2>&1 | grep version  # should say 'gcc version 4.6.1 (GCC)'
     valgrind --version          # should say 'valgrind-3.6.1'
     igprof -h                   # should print simple help message
     which igprof-navigator      # should say full path

6. Create a web area where you will put output from some exercises::

     mkdir -p /storage/nfs_esc11/stud01/public_html/cgi-bin/data
     ln -s /storage/nfs_esc11/stud01/public_html ~/public_html
     cp $(which igprof-navigator) ~/public_html/cgi-bin/igprof-navigator.py
     echo "<html><body><a href='cgi-bin/igprof-navigator.py'>My" \
       "igprof reports</a></body></html>" > ~/public_html/index.html

7. View http://137.204.203.67/~stud01/ in your web browser, you should see
   the basic page you created above. Click on the link to profiles, it should
   produce some output -- for now it will in fact display an error message
   due to lack of profiles.

8. You should be able to view some of these CMS profile examples:
   `perf report <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias02_perf/>`_,
   `heap snapshot after 50 events <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03.50_live/>`_,
   `total dynamic memory allocations <http://cern.ch/cms-service-sdtweb/igperf/vocms81/slc5_ia32_gcc434/360p1/navigator/minbias03_total/>`_.

In subsequent exercises, you should have one more terminal windows open
with ssh session to the student server, and source the environment setup
script. You will normally use ``env-gcc461.sh``, but in some cases you
will also use ``env-gcc451.sh``. **Always start each new exercise in a
fresh new shell environment!**

If you are adventurous, you can build igprof standalone yourself following
the `recipe on the web site <http://igprof.sourceforge.net/install.html>`_.
