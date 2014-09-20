require 'rist'
Bundler.require(:development)

Dir[File.absolute_path("spec/support/**/*.rb")].each { |f| require f }

RSpec.configure do |config|
end
