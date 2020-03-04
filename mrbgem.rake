MRuby::Gem::Specification.new('mruby-esp32-storage') do |spec|
    spec.license = 'MIT'
    spec.author  = 'KAGEYAMA Katsuhiko'
    spec.summary = 'Storage module for mruby-esp32'

    spec.add_dependency('mruby-io', :github => 'kishima/mruby-io', :branch => 'esp32')

end

