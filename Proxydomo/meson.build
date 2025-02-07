boost_dep = dependency(
    'boost',
    # static: true,
    modules: [
        'thread',
        'date_time',
        'timer',
        'log',
        'filesystem',
    ],
)

icuuc_dep = dependency('icu-uc')
icui18n_dep = dependency('icu-i18n')
icuio_dep = dependency('icu-io')
icudata_dep = subproject('icu').get_variable('icudata_dep')

zlib_dep = dependency('zlib')
openssl_dep = dependency('openssl', default_options: ['default_library=static'])

args = [
    '/source-charset:.932',
    '-DWINVER=0x0A00',
    # '-D_WIN32_WINNT=0x0A00',
    # '-D_WIN32_WINNT=0x0501',
    '-DUNICODE',
    '-D_UNICODE',
    '-DWIN32_LEAN_AND_MEAN',
    '-DUCHAR_TYPE=wchar_t',
    '-DWOLFSSL_CERT_GEN',
    '-DWOLFSSL_KEY_GEN',
    '-DHAVE_AESGCM',
    '-DHAVE_AESCCM',
    '-DHAVE_CAMELLIA',
    '-DHAVE_ECC',
    '-DHAVE_OCSP',
    '-DHAVE_TLS_EXTENSIONS',
    '-DHAVE_SESSION_TICKET',
    '-DHAVE_SNI',
    '-DHAVE_SECURE_RENEGOTIATION',
    '-DHAVE_SUPPORTED_CURVES',
    '-DWOLFSSL_RIPEMD',
    '-DWOLFSSL_SHA384',
    '-DWOLFSSL_SHA512',
    '-DWOLFSSL_STATIC_RSA',
    '-DNO_RC4',
    '-DNO_PSK',
    '-DWOLFSSL_ALT_NAMES',
    '-DWOLFSSL_ALT_NAMES',
    '-DWIN32',
    '-D_WINDOWS',
    '-DSTRICT',
    '-DNOMINMAX',
    '-D_DEBUG',
]

windows = import('windows')
executable(
    'proximodo',
    [
        windows.compile_resources('Proxydomo.rc', include_directories: wtl_inc),
        'AdblockFilter.cpp',
        'BlockListDatabase.cpp',
        'BrotliDecompressor.cpp',
        'brotli/common/dictionary.c',
        'brotli/dec/bit_reader.c',
        'brotli/dec/decode.c',
        'brotli/dec/huffman.c',
        'brotli/dec/state.c',
        'brotli/enc/backward_references.c',
        'brotli/enc/backward_references_hq.c',
        'brotli/enc/bit_cost.c',
        'brotli/enc/block_splitter.c',
        'brotli/enc/brotli_bit_stream.c',
        'brotli/enc/cluster.c',
        'brotli/enc/compress_fragment.c',
        'brotli/enc/compress_fragment_two_pass.c',
        'brotli/enc/dictionary_hash.c',
        'brotli/enc/encode.c',
        'brotli/enc/entropy_encode.c',
        'brotli/enc/histogram.c',
        'brotli/enc/literal_cost.c',
        'brotli/enc/memory.c',
        'brotli/enc/metablock.c',
        'brotli/enc/static_dict.c',
        'brotli/enc/utf8_util.c',
        'CodeConvert.cpp',
        'ConnectionMonitor.cpp',
        'DebugWindow.cpp',
        'DomainJudge.cpp',
        'FilterDescriptor.cpp',
        'FilterEditWindow.cpp',
        'FilterManageWindow.cpp',
        'FilterOwner.cpp',
        'Logger.cpp',
        'LogViewWindow.cpp',
        'MainDlg.cpp',
        'Matcher_.cpp',
        'Misc.cpp',
        'Nodes_.cpp',
        # 'proximodo/configscreen.cpp',
        # 'proximodo/controls.cpp',
        # 'proximodo/descriptor.cpp',
        # 'proximodo/editscreen.cpp',
        # 'proximodo/events.cpp',
        'proximodo/expander.cpp',
        'proximodo/filter.cpp',
        # 'proximodo/filterowner.cpp',
        # 'proximodo/giffilter.cpp',
        'proximodo/headerfilter.cpp',
        # 'proximodo/log.cpp',
        # 'proximodo/logframe.cpp',
        # 'proximodo/main.cpp',
        # 'proximodo/mainframe.cpp',
        # 'proximodo/managerthread.cpp',
        # 'proximodo/matcher.cpp',
        # 'proximodo/nodes.cpp',
        # 'proximodo/platform.cpp',
        # 'proximodo/proxy.cpp',
        # 'proximodo/requestmanager.cpp',
        # 'proximodo/settings.cpp',
        # 'proximodo/settingsscreen.cpp',
        # 'proximodo/testframe.cpp',
        # 'proximodo/textbuffer.cpp',
        'proximodo/textfilter.cpp',
        # 'proximodo/trayicon.cpp',
        'proximodo/url.cpp',
        'proximodo/util.cpp',
        # 'proximodo/welcomescreen.cpp',
        # 'proximodo/windowcontent.cpp',
        'proximodo/zlibbuffer.cpp',
        'Proxy.cpp',
        'Proxydomo.cpp',
        'RequestManager.cpp',
        'Settings.cpp',
        'Socket.cpp',
        'ssl.cpp',
        'stdafx.cpp',
        'TextBuffer.cpp',
        'UITranslator.cpp',
        # 'UnitTest/Test.cpp',
        # 'UnitTest/Test_brotli.cpp',
        'VersionControl.cpp',
        'WinHTTPWrapper.cpp',
    ],
    include_directories: include_directories('brotli/include'),
    install: true,
    gui_app: true,
    dependencies: [
        wtl_dep,
        boost_dep,
        zlib_dep,
        icui18n_dep,
        icuuc_dep,
        icuio_dep,
        icudata_dep,
        openssl_dep,
        # libcrypto_dep,
        # libssl_dep,
        sqlite3_dep,
    ],
    cpp_args: args,
    c_args: args,
)

install_subdir(
    'proximodo/distrib',
    strip_directory: true,
    install_dir: 'bin',
    exclude_directories: 'lists/CVS',
)
install_subdir(
    'lang',
    install_dir: 'bin',
)
