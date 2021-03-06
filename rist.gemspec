# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'rist/version'

Gem::Specification.new do |spec|
  spec.name          = "rist"
  spec.version       = Rist::VERSION
  spec.authors       = ["Zhi-Qiang Lei"]
  spec.email         = ["zhiqiang.lei@gmail.com"]
  spec.summary       = %q{Ruby Intelligent System Tools}
  spec.description   = %q{}
  spec.homepage      = ""
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0") - Dir['{examples,spec}/**/*'] - %w{.gitignore Gemfile Rakefile}
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]
  spec.extensions    = %w{ext/transcriber/extconf.rb}

  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rspec", "~> 3.1"
  spec.add_development_dependency "factory_girl", "~> 4.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "coveralls"
end
