const getFileName = require("prompt-sync")()


function main (): number {
    console.clear()
    //console.log("PROJETO DE LINGUAGENS FORMAIS E AUTOMATOS")
    //console.log("\nAUTOR: Andrei Pochmann Koenich\n")
    //console.log("-----------------------------------------------\n")
    readInputFile()
    return 0
}

function controlFileData(fileLines: string[]): void {

    const firstLineRegex:   RegExp = /.+=\({(.,)*.},{((.,)*.)*},.+,.\)/
    if (!firstLineRegex.test(fileLines[0]))
        throw new Error("erro de sintaxe na primeira linha do arquivo de entrada.");
    
    const secondLineRegex:  RegExp = /.+/
    if (!secondLineRegex.test(fileLines[1])) 
        throw new Error("erro de sintaxe na segunda linha do arquivo de entrada.");

    const otherLinesRegex:  RegExp = /. ->( .)?/
    for (let i: number = 2; i < fileLines.length; i++)
        if (!otherLinesRegex.test(fileLines[i])) 
            throw new Error(`erro de sintaxe na linha ${i+1} do arquivo de entrada.`);
}

function getFileData(data: string): void {
    let fileLines: string[] = data.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")

    controlFileData(fileLines);
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