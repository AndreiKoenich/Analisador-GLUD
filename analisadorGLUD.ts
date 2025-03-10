const getFileName = require("prompt-sync")()

const firstLineRegex:   RegExp = /.+=\({([a-zA-Z0-9],)*[a-zA-Z0-9]},{(([a-zA-Z0-9],)*[a-zA-Z0-9])*},[a-zA-Z0-9]+,[a-zA-Z0-9]\)/
const secondLineRegex:  RegExp = /[a-zA-Z0-9]+/
const otherLinesRegex:  RegExp = /[a-zA-Z0-9] ->( [a-zA-Z0-9])?/

class InputFileInfo {
    grammarName: string = ""
    variables: string[] = []
    terminals: string[] = []
    productionsName: string = ""
    initialVariable: string = ""
    productions: string[] = []
}

function main (): number {
    console.clear()
    //console.log("PROJETO DE LINGUAGENS FORMAIS E AUTOMATOS")
    //console.log("\nAUTOR: Andrei Pochmann Koenich\n")
    //console.log("-----------------------------------------------\n")
    readInputFile()
    return 0
}

function getFileInfo(fileFirstLine: string): InputFileInfo {

    let fileInfo = new InputFileInfo();
    const splitInfo: string[] = fileFirstLine.split(/[{}]/)

    fileInfo.grammarName = splitInfo[0].replace("=(","")
    fileInfo.variables = splitInfo[1].split(",")
    fileInfo.terminals = splitInfo[3].split(",")
    for (let i: number = 1; splitInfo[4][i] != ","; i++)
        fileInfo.productionsName += splitInfo[4][i]
    fileInfo.initialVariable = splitInfo[4].split(/[,)]/)[2]

    return fileInfo
}

function controlFileData(fileLines: string[]): void {

    if (!firstLineRegex.test(fileLines[0])) {
        console.log("Erro de sintaxe na primeira linha do arquivo de entrada.");
        process.exit(1);
    }
    
    if (!secondLineRegex.test(fileLines[1])) {
        console.log("Erro de sintaxe na segunda linha do arquivo de entrada.");
        process.exit(1);
    }

    for (let i: number = 2; i < fileLines.length; i++)
        if (!otherLinesRegex.test(fileLines[i])) {
            console.log(`Erro de sintaxe na linha ${i+1} do arquivo de entrada.`);
            process.exit(1);
        }
}

function getFileData(data: string): void {
    let fileLines: string[] = data.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")

    controlFileData(fileLines);
    let fileInfo: InputFileInfo = getFileInfo(fileLines[0]);
}

function readInputFile(): void {
    //console.log("Digite o nome do arquivo de entrada, contendo a GLUD:")
    //const fileName = getFileName()

    const fileName = "glud_teste.txt"

    const fs: any = require("fs")
    fs.readFile(fileName, (err: any, data: string) => {
        if (err) throw err;
        getFileData(data)
    })

}

main()