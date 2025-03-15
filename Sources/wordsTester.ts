import { AutomataVertex, InputFileInfo } from "./classes"
import { onlyWhiteSpacesRegex } from "./constants"

export function testWords (wordsFileData: string, vertexesDA: AutomataVertex[], grammarInfo: InputFileInfo): void {

    const fileLines: string[] = wordsFileData.toString().split("\n")

    for (let i: number = 0; i < fileLines.length; i++)
       fileLines[i] = fileLines[i].replace("\r","")
    
    console.log("\nPalavras do arquivo de entrada que podem ser geradas pela GLUD:")
    for (let i: number = 0; i < fileLines.length; i++) 
        testSingleWord(fileLines[i], vertexesDA, grammarInfo)
} 

function testSingleWord (word: string, vertexesDA: AutomataVertex[], grammarInfo: InputFileInfo): void {

    if (onlyWhiteSpacesRegex.test(word)) {
        
        if (grammarInfo.acceptsEmptyWord) {
            console.log("<palavra vazia>")
            return         
        }

        else 
            return
    }

    let vertexIndex: number = grammarInfo.variables.indexOf(grammarInfo.initialVariable)
    let currentState: string
    
    for (let i: number = 0; i < word.length; i++) {
        
        let acceptWord: boolean = false

        for (let j: number = 0; j < vertexesDA[vertexIndex].edges.length; j++) {

            if (vertexesDA[vertexIndex].edges[j][0] === word[i]) {

                if (vertexesDA[vertexIndex].edges[j].length === 1) {

                    if (i === word.length-1) {
                        console.log(word)
                        return
                    }

                    else
                        return
                }

                currentState = vertexesDA[vertexIndex].edges[j].slice(1)
                vertexIndex = grammarInfo.variablesSubsets.indexOf(currentState.split("").sort().join(""))
                acceptWord = true
                break
            }
        }

        if (!acceptWord)
            return     
    }

    if (vertexesDA[vertexIndex].isFinalState)
        console.log(word)
}