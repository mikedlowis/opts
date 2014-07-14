require 'rscons'
require 'rbconfig'

is_windows = (RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)

desc "Force the build to build in Posix mode"
task(:posix){ is_windows = false }

base = Rscons::Environment.new do |env|
  #env["CFLAGS"] += ['-Wall', '-Werror']
  env['CXXSUFFIX'] = '.cpp'
end

task :default => [:staticlib]

task :build => [:staticlib]

task :test => [:unittest_pp] do
    base.clone do |env|
        env['CPPFLAGS'] << 'tools/UnitTest++/src/'
        env.Program('build/test_libopts', Dir['source/**/*.{c,cpp}', "tests/**/*.{c,cpp}"])
    end.process
end

desc "Build the OPTS static library"
task :staticlib do
    base.Library('build/libopts.a', Dir['source/**.{c,cpp}'])
    base.process
end

task :unittest_pp do
    sources = Dir['tools/UnitTest++/src/*.{c,cpp}']
    if is_windows
        sources += Dir['tools/UnitTest++/src/Win32/*.{c,cpp}']
    else
        sources += Dir['tools/UnitTest++/src/Posix/*.{c,cpp}']
    end
    base.Library('build/UnitTest++.a', sources)
    base.process
end

desc "Clean all generated files and directories"
task(:clean) { Rscons.clean }
