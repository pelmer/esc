### RPM cms esc 2011b

%define webdoc_files %i/doc/
#Source: https://github.com/lat/esc/tarball/%{realversion}
Source: https://github.com/lat/esc/tarball/3a323e250f801dfab6431be18a3725dd96ace994
Requires: py2-sphinx igprof valgrind gdb

%prep
%setup -n lat-esc-3a323e2

%build
cd doc
make html

%install
mkdir -p %i/{doc,etc/profile.d}
rsync -av --delete doc/.build/html/ %i/doc/

# Generate dependencies-setup.{sh,csh} so init.{sh,csh} picks full environment.
: > %i/etc/profile.d/dependencies-setup.sh
: > %i/etc/profile.d/dependencies-setup.csh
for tool in $(echo %{requiredtools} | sed -e's|\s+| |;s|^\s+||'); do
  root=$(echo $tool | tr a-z- A-Z_)_ROOT; eval r=\$$root
  if [ X"$r" != X ] && [ -r "$r/etc/profile.d/init.sh" ]; then
    echo "test X\$$root != X || . $r/etc/profile.d/init.sh" >> %i/etc/profile.d/dependencies-setup.sh
    echo "test X\$$root != X || source $r/etc/profile.d/init.csh" >> %i/etc/profile.d/dependencies-setup.csh
  fi
done

%post
%{relocateConfig}etc/profile.d/*.*sh

%files
%i/
%exclude %i/doc

## SUBPACKAGE webdoc
