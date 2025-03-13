import * as Constants from "./constants"
import { InputFileInfo, AutomataVertex } from "./classes"

function hasDuplicateElements (stringArray: string[]) : boolean {
    return new Set(stringArray).size != stringArray.length 
}

export function printGrammarInfo (grammarInfo: InputFileInfo) : void {
    console.log(grammarInfo.grammarName)
    console.log(grammarInfo.initialVariable)
    console.log(grammarInfo.variables)
    console.log(grammarInfo.terminals)
    console.log(grammarInfo.productionsName)
    console.log(grammarInfo.productionRules)
}

export function controlFileSyntax(fileLines: string[]): void {

    if (!Constants.firstLineRegex.test(fileLines[0])) {
        console.log("ERRO: sintaxe da primeira linha do arquivo de entrada incorreta.")
        process.exit(1)
    }
    
    if (!Constants.secondLineRegex.test(fileLines[1])) {
        console.log("ERRO: sintaxe da segunda linha do arquivo de entrada incorreta.")
        process.exit(1)
    }

    for (let i: number = 2; i < fileLines.length; i++)
        if (!Constants.otherLinesRegex.test(fileLines[i])) {
            console.log(`ERRO: sintaxe da linha ${i+1} do arquivo de entrada incorreta.`)
            process.exit(1)
        }
}

export function isAlphaOrDigit (character: string) : boolean {
    return (Constants.alphaOrDigitRegex.test(character))
}

function testStringArrayIntersection(a: string[], b: string[]) : boolean {
    const s = new Set(b);
    return [...new Set(a)].some((x) => s.has(x));
};

export function controlGrammarFileData(grammarInfo: InputFileInfo, secondLineInfo: string) : void {

    if(hasDuplicateElements(grammarInfo.variables)) {
        console.log("ERRO: o conjunto das variaveis possui elementos duplicados, na primeira linha do arquivo de entrada.")
        process.exit(1)
    }

    if(hasDuplicateElements(grammarInfo.terminals)) {
        console.log("ERRO: o conjunto dos terminais possui elementos duplicados, na primeira linha do arquivo de entrada.")
        process.exit(1)
    }

    if (grammarInfo.productionsName != secondLineInfo) {
        console.log("ERRO: o nome das regras de producao da primeira linha nao corresponde ao nome das regras na segunda linha, no arquivo de entrada.")
        console.log("Nome na primeira linha:", grammarInfo.productionsName)
        console.log("Nome na segunda linha:", secondLineInfo)
        process.exit(1)
    }

    if (grammarInfo.productionRules.length == 0) {
        console.log("ERRO: nenhuma regra de producao informada no arquivo de entrada.")
        process.exit(1)
    }

    if (testStringArrayIntersection(grammarInfo.variables,grammarInfo.terminals)) {
        console.log("ERRO: na primeira linha do arquivo de entrada, existem nomes de variaveis incluidos no conjunto dos terminais, ou vice-versa.")
        process.exit(1)
    }

    for (let i: number = 0; i < grammarInfo.productionRules.length; i++) {
        if (!grammarInfo.variables.includes(grammarInfo.productionRules[i][0])) {
            console.log(`ERRO: variavel '${grammarInfo.productionRules[i][0]}' do lado esquerdo da regra de producao da linha ${i+3} invalida.`)
            process.exit(1)
        }

        if (grammarInfo.productionRules[i].length >= 6) {
            if (isAlphaOrDigit(grammarInfo.productionRules[i][5]) && !grammarInfo.terminals.includes(grammarInfo.productionRules[i][5])) {
                console.log(`ERRO: terminal '${grammarInfo.productionRules[i][5]}' do lado direito da regra de producao da linha ${i+3} invalido.`)
                process.exit(1)
            }

            if (grammarInfo.productionRules[i].length == 7) {
                if (isAlphaOrDigit(grammarInfo.productionRules[i][6]) && !grammarInfo.variables.includes(grammarInfo.productionRules[i][6])) {
                    console.log(`ERRO: variavel '${grammarInfo.productionRules[i][6]}' do lado direito da regra de producao da linha ${i+3} invalida.`)
                    process.exit(1)
                }
            }
        }
    } 
}