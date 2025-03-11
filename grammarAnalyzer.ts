import { controlGrammarFileData, controlFileSyntax } from "./controlGrammarFileData"
import { buildAutomata } from "./buildAutomata"
import { AutomataVertex, InputFileInfo } from "./classes"

function main (): number {

    console.clear()
    console.log("PROJETO DE LINGUAGENS FORMAIS E AUTOMATOS")
    console.log("\nAUTOR: Andrei Pochmann Koenich\n")
    console.log("-----------------------------------------------\n")
    readInputFiles()
    return 0
}

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

function getFilegrammarFileData(grammarFileData: string): AutomataVertex[] {
    const fileLines: string[] = grammarFileData.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")

    controlFileSyntax(fileLines)
    let grammarInfo: InputFileInfo = getFirstLineInfo(fileLines[0])
    const secondLineInfo: string = fileLines[1]
    for (let i: number = 2; i < fileLines.length; i++)
        grammarInfo.productionRules.push(fileLines[i])

    controlGrammarFileData(grammarInfo, secondLineInfo)
    let vertexes: AutomataVertex[] = buildAutomata(grammarInfo)
    return vertexes
    
}

function readInputFiles(): void {
    const prompt = require("prompt-sync")()

    let vertexes: AutomataVertex[] = []

    console.log("Digite o nome do arquivo texto de entrada, contendo as definicoes da GLUD:")
    let grammarFileName = prompt()
    const fs = require("fs")
    try {
        const grammarFileData = fs.readFileSync(grammarFileName, "utf-8")
        vertexes = getFilegrammarFileData(grammarFileData)
    } catch (err) {
        console.error("Erro ao ler o arquivo contendo as definicoes da GLUD:", err)
    }

    console.log("\nDigite o nome do arquivo texto de entrada contendo as palavras a serem testadas:")
    let wordsFileName = prompt()
    try {
        const wordsFileData = fs.readFileSync(wordsFileName, "utf-8")
        console.log(wordsFileData)
    } catch (err) {
        console.error("Erro ao ler o arquivo contendo as palavras a serem testadas:", err)
    }
}

main()