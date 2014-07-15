require 'rscons'
require 'rbconfig'

is_windows = (RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)

desc "Force the build to build in Posix mode"
task(:posix){ is_windows = false }

base = Rscons::Environment.new do |env|
  env["CFLAGS"] += ['-Wall', '-Werror']
  env['CXXSUFFIX'] = '.cpp'
end

task :default => [:test, :build]

task :build => [:staticlib]

desc "Run all unit tests"
task :test => [:unittest_pp] do
    base.clone do |env|
        env['CPPPATH'] += ['tools/UnitTest++/src/', 'source/']
        env.Program('build/test_libopts', Dir['source/**/*.{c,cpp}', "tests/**/*.{c,cpp}", 'build/UnitTest++.a'])
     end.process
     sh "build/test_libopts"
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
