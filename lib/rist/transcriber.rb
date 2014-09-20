require 'transcriber/transcriber'

class Rist::Transcriber

  # @param [Hash] options the options to initialize a transcriber identical to PocketSphinx arguments
  def initialize(options = {})
    arguments = options.inject([]) { |result, (key, value)| result + ["-#{key.to_s}", value.to_s] }
    initialize_pocketsphinx(arguments);
  end

end
