require 'transcriber/transcriber'

class Rist::Transcriber
  def initialize(options = {})
    arguments = options.inject([]) { |result, (key, value)| result + ["-#{key.to_s}", value.to_s] }
    initialize_pocketsphinx(arguments);
  end
end
