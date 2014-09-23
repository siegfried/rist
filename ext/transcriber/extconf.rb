require 'mkmf'

unless have_framework(%w{OpenAL alc.h})
  unless find_header 'AL/al.h', '/usr/local/include', '/usr/include'
    abort 'cannot find OpenAL, please install it.'
  end
  unless find_header 'AL/alc.h', '/usr/local/include', '/usr/include'
    abort 'cannot find OpenAL, please install it.'
  end
  find_library 'openal', 'alcCaptureOpenDevice'
end

unless find_header 'pocketsphinx/pocketsphinx_export.h', '/usr/local/include', '/opt/include', '/usr/include'
  abort 'cannot find pocketsphinx, please install it.'
end
unless find_header 'sphinxbase/sphinxbase_export.h', '/usr/local/include', '/opt/include', '/usr/include'
  abort 'cannot find sphinxbase, please install it.'
end
find_header 'pocketsphinx_export.h', '/usr/local/include/pocketsphinx', '/opt/include/pocketsphinx', '/usr/include/pocketsphinx'
find_header 'sphinxbase_export.h', '/usr/local/include/sphinxbase', '/opt/include/sphinxbase', '/usr/include/sphinxbase'

find_library 'pocketsphinx', 'ps_init', '/usr/local/lib', '/opt/lib', '/usr/lib'
find_library 'sphinxbase', 'cmd_ln_init', '/usr/local/lib', '/opt/lib', '/usr/lib'

create_header
create_makefile 'transcriber/transcriber'
