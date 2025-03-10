import * as Classes from "./classes"
import { controlFileData, controlFileSyntax, printGrammarInfo } from "./controlFileData"

function main (): number {
    console.clear()
    //console.log("PROJETO DE LINGUAGENS FORMAIS E AUTOMATOS")
    //console.log("\nAUTOR: Andrei Pochmann Koenich\n")
    //console.log("-----------------------------------------------\n")
    readInputFile()
    return 0
}

function getFirstLineInfo(fileFirstLine: string): Classes.InputFileInfo {

    let fileInfo = new Classes.InputFileInfo()
    const splitInfo: string[] = fileFirstLine.split(/[{}]/)

    fileInfo.grammarName = splitInfo[0].replace("=(","")
    fileInfo.variables = splitInfo[1].split(",")
    fileInfo.terminals = splitInfo[3].split(",")
    for (let i: number = 1; splitInfo[4][i] != ","; i++)
        fileInfo.productionsName += splitInfo[4][i]
    fileInfo.initialVariable = splitInfo[4].split(/[,)]/)[2]

    return fileInfo
}

function getFileData(data: string): void {
    const fileLines: string[] = data.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")

    controlFileSyntax(fileLines)
    let grammarInfo: Classes.InputFileInfo = getFirstLineInfo(fileLines[0])
    const secondLineInfo: string = fileLines[1]
    for (let i: number = 2; i < fileLines.length; i++)
        grammarInfo.productionRules.push(fileLines[i])

    controlFileData(grammarInfo, secondLineInfo)
    printGrammarInfo(grammarInfo)
}

function readInputFile(): void {
    //console.log("Digite o nome do arquivo de entrada, contendo a GLUD:")
    //const fileName = getFileName()

    const fileName = "glud_teste.txt"
    let grammarInfo: Classes.InputFileInfo;

    const fs: any = require("fs")
    fs.readFile(fileName, (err: any, data: string) => {
        if (err) throw err;
        getFileData(data)
    })
}

main()