cmake_minimum_required(VERSION 2.8)

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 NEW)
endif()

set(EXAMPLES
    capitalize
    condense
    filterkey
    filterkeydom
    jsonx
    lookaheadparser
    messagereader
    parsebyparts
    pretty
    prettyauto
    schemavalidator
    serialize
    simpledom
    simplereader
    simplepullreader
    simplewriter
    sortkeys
    tutorial)
    

