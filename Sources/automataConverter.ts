import { AutomataVertex, InputFileInfo } from "./classes"
import { removeDuplicates, isAlphaOrDigit } from "./utils"

export function removeNonDeterminism (vertexesND: AutomataVertex[], grammarInfo: InputFileInfo): AutomataVertex[] {

    let vertexesD: AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variablesSubsets.length; i++)
        vertexesD.push(new AutomataVertex(grammarInfo.variablesSubsets[i],[], false))

    for (let i: number = 0; i < grammarInfo.terminals.length; i++) {

        for (let j: number = 0; j < grammarInfo.variablesSubsets.length; j++) {

            let edgeAux: string = grammarInfo.terminals[i]
            let containsTerminal: boolean = false
            let isFinalState: boolean = false

            for (let k: number = 0; k < grammarInfo.variablesSubsets[j].length; k++) {

                let vertexIndex: number = grammarInfo.variables.indexOf(grammarInfo.variablesSubsets[j][k])

                if (vertexesND[vertexIndex].isFinalState)
                    isFinalState = true

                if (vertexesND[vertexIndex].edges.join("").includes(grammarInfo.terminals[i]))
                    containsTerminal = true
    
                for (let l: number = 0; l < vertexesND[vertexIndex].edges.length; l++) {

                    if (vertexesND[vertexIndex].edges[l][0] == grammarInfo.terminals[i] // RAZAO DO ERRO
                        && vertexesND[vertexIndex].edges[l].length > 1 
                        && isAlphaOrDigit(vertexesND[vertexIndex].edges[l][1])) {
                            edgeAux += vertexesND[vertexIndex].edges[l][1]
                    }
                }
            } 

            if (containsTerminal) {
                edgeAux = removeDuplicates(edgeAux)
                vertexesD[j].edges.push(edgeAux)
                vertexesD[j].isFinalState = isFinalState
            } 
        }
    }

    return vertexesD
}