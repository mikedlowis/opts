#------------------------------------------------------------------------------
# Bundler Setup
#------------------------------------------------------------------------------
require "bundler"
begin
  Bundler.setup(:default, :development)
rescue Bundler::BundlerError => e
  raise LoadError.new("Unable to Bundler.setup(): You probably need to run `bundle install`: #{e.message}")
end
require 'rscons'
require 'rbconfig'

#------------------------------------------------------------------------------
# Envrionment Definitions
#------------------------------------------------------------------------------
# Define the compiler environment
Env = Rscons::Environment.new do |env|
  env.build_dir('source/','build/obj/source')
  env["CFLAGS"] += ['-Wall', '-Wextra', ]#'-Werror']
  env['CPPPATH'] += Dir['source/**/']
end

# Define the test environment
TestEnv = Env.clone  do |env|
  env.build_dir('source','build/obj/test_source')
  env.build_dir('tests','build/obj/tests/source')
  env['CPPPATH'] += Dir['tests/']
  env["CFLAGS"]  += ['--coverage']
  env["LDFLAGS"] += ['--coverage']
end

# Make sure the environment is processed before we quit
at_exit { Env.process; TestEnv.process}

#------------------------------------------------------------------------------
# Main Build Targets
#------------------------------------------------------------------------------
task :default => [:test, :build]

desc "Build the Opts static library"
task :build do
    Env.Library('build/libopts.a', Dir['source/**/*.c'])
    Env.process
end

#------------------------------------------------------------------------------
# Unit Testing Targets
#------------------------------------------------------------------------------
desc "Run all unit tests"
task :test do
    TestEnv.Program('build/test_libopts', Dir['source/**/*.c', 'tests/**/*.c'])
    TestEnv.process
    sh "build/test_libopts"
end

#------------------------------------------------------------------------------
# Cleanup Target
#------------------------------------------------------------------------------
desc "Clean all generated files and directories"
task(:clean) { Rscons.clean }

