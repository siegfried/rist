require 'mkmf'

header_paths = ['/usr/local/include', '/opt/include', '/usr/include']

unless find_header 'pocketsphinx/pocketsphinx_export.h', *header_paths
  abort 'cannot find pocketsphinx, please install it.'
end
unless find_header 'sphinxbase/sphinxbase_export.h', *header_paths
  abort 'cannot find sphinxbase, please install it.'
end
find_header 'pocketsphinx_export.h', '/usr/local/include/pocketsphinx', '/opt/include/pocketsphinx', '/usr/include/pocketsphinx'
find_header 'sphinxbase_export.h', '/usr/local/include/sphinxbase', '/opt/include/sphinxbase', '/usr/include/sphinxbase'

lib_paths = ['/usr/local/lib', '/opt/lib', '/usr/lib']

find_library 'pocketsphinx',  'ps_init',      *lib_paths
find_library 'sphinxbase',    'cmd_ln_init',  *lib_paths
find_library 'sphinxad',      'ad_open_dev',  *lib_paths

create_header
create_makefile 'transcriber/transcriber'
