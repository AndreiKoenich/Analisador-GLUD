export const getFileName = require("prompt-sync")()
export const firstLineRegex:   RegExp = /.+=\({([a-zA-Z0-9],)*[a-zA-Z0-9]},{(([a-zA-Z0-9],)*[a-zA-Z0-9])*},[a-zA-Z0-9]+,[a-zA-Z0-9]\)/
export const secondLineRegex:  RegExp = /[a-zA-Z0-9]+/
export const otherLinesRegex:  RegExp = /[a-zA-Z0-9] ->( [a-zA-Z0-9])?/
export const alphaOrDigitRegex: RegExp = /[a-zA-Z0-9]/