const getFileName = require("prompt-sync")();

function main (): number {
    console.clear();
    console.log("PROJETO DE LINGUAGENS FORMAIS E AUTOMATOS")
    console.log("\nAUTOR: Andrei Pochmann Koenich\n")
    console.log("-----------------------------------------------\n")
    iniciaLeitura()
    return 0
}

function iniciaLeitura(): void {
    console.log("Digite o nome do arquivo de entrada, contendo a GLUD:")
    const fileName = getFileName()

    const fs = require("fs")
    fs.readFile(fileName, (err: any, data: any) => {
        if (err) throw err;
        console.log(data.toString());
    });

}

main()