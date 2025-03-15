export const getFileName = require("prompt-sync")()

export const firstLineRegex:   RegExp = /^.+=\({([a-zA-Z0-9],)*[a-zA-Z0-9]},{(([a-zA-Z0-9],)*[a-zA-Z0-9])*},[a-zA-Z0-9]+,[a-zA-Z0-9]\)$/
export const secondLineRegex:  RegExp = /^[a-zA-Z0-9]+$/
export const otherLinesRegex:  RegExp = /^[a-zA-Z0-9] ->(( [a-zA-Z0-9]?[a-zA-Z0-9](\s*))|\s*)$/
export const alphaOrDigitRegex: RegExp = /^[a-zA-Z0-9]$/
export const onlyWhiteSpacesRegex: RegExp = /^[ \t]*$/

export const minorProductionRuleSize: number = 6
export const majorProductionRuleSize: number = 7

export const emptyLanguageCode: number = 0
export const finiteLanguageCode: number = 1
export const infiniteLanguageCode: number = 2