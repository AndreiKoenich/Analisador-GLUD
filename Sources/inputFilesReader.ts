import { controlGrammarFileData, controlFileSyntax } from "./fileDataController"
import { buildNonDeterministicAutomata } from "./automataBuilder"
import { AutomataVertex, InputFileInfo } from "./classes"
import { testWords } from "./wordsTester"
import { removeNonDeterminism } from "./automataConverter"
import { getSubsets } from "./utils"

function getFirstLineInfo(fileFirstLine: string): InputFileInfo {

    let fileInfo = new InputFileInfo()
    const splitInfo: string[] = fileFirstLine.split(/[{}]/)

    fileInfo.grammarName = splitInfo[0].replace("=(","")
    fileInfo.variables = splitInfo[1].split(",")
    fileInfo.terminals = splitInfo[3].split(",")
    for (let i: number = 1; splitInfo[4][i] != ","; i++)
        fileInfo.productionsName += splitInfo[4][i]
    fileInfo.initialVariable = splitInfo[4].split(/[,)]/)[2]

    return fileInfo
}

function getInputFileData(grammarFileData: string): InputFileInfo {
    const fileLines: string[] = grammarFileData.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")

    controlFileSyntax(fileLines)
    let grammarInfo: InputFileInfo = getFirstLineInfo(fileLines[0])
    const secondLineInfo: string = fileLines[1]
    for (let i: number = 2; i < fileLines.length; i++)
        grammarInfo.productionRules.push(fileLines[i])

    grammarInfo.variablesSubsets = getSubsets(grammarInfo.variables)

    controlGrammarFileData(grammarInfo, secondLineInfo)
    return grammarInfo
}

export function readInputFiles(): void {
    const prompt = require("prompt-sync")()

    let grammarInfo = new InputFileInfo()

    console.log("Digite o nome do arquivo texto de entrada, contendo as definicoes da GLUD:")
    let grammarFileName = prompt()
    const fs = require("fs")
    try {
        const grammarFileData = fs.readFileSync(grammarFileName, "utf-8")
        grammarInfo = getInputFileData(grammarFileData)
    } catch (err) {
        console.error("Erro ao ler o arquivo contendo as definicoes da GLUD:", err)
    }

    let vertexesND: AutomataVertex[] = buildNonDeterministicAutomata(grammarInfo)

    if (vertexesND[grammarInfo.variables.indexOf(grammarInfo.initialVariable)].isFinalState)
        grammarInfo.acceptsEmptyWord = true

    let vertexesD: AutomataVertex[] = removeNonDeterminism(vertexesND, grammarInfo)

    console.log("\nDigite o nome do arquivo texto de entrada contendo as palavras a serem testadas:")
    let wordsFileName = prompt()
    let wordsFileData: string = ""
    try {
        wordsFileData = fs.readFileSync(wordsFileName, "utf-8")
    } catch (err) {
        console.error("Erro ao ler o arquivo contendo as palavras a serem testadas:", err)
    }

    testWords(wordsFileData, vertexesD, grammarInfo)
}