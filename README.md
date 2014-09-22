# Rist

[![Gem Version](https://badge.fury.io/rb/rist.svg)](http://badge.fury.io/rb/rist)

Rist (Ruby Intelligent System Tools) is a tool set to build Intelligent System.

## Installation

*Warning: OpenAL and Pocketsphinx (> 0.8) are prerequisite.*

Add this line to your application's Gemfile:

```ruby
gem 'rist'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install rist

## Usage

### Transcriber

Transcriber captures audio using OpenAL and turns the audio into text using [Pocketsphinx](http://cmusphinx.sourceforge.net/).

```ruby
transcriber = Rist::Transcriber.new logfn: "/dev/null"

transcriber.transcribe do |utterance|
  puts utterance
end
```

## Contributing

1. Fork it ( https://github.com/siegfried/rist/fork )
2. Create your feature branch (`git checkout -b my-new-feature`)
3. Commit your changes (`git commit -am 'Add some feature'`)
4. Push to the branch (`git push origin my-new-feature`)
5. Create a new Pull Request
