$: << File.absolute_path('lib')

require 'rist'

include Rist

transcriber = Transcriber.new logfn: "/dev/null"

transcriber.transcribe do |utterance|
  puts utterance
end
