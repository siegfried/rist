RSpec.describe Rist::Transcriber do
  describe '.new' do
    subject { described_class.new }
    it { should be_a described_class }
  end

  describe '.new(options)' do
    subject { described_class.new options }
    let(:options) { { logfn: "/dev/null" } }
    it { should be_a described_class }
  end
end
