import { AutomataVertex, InputFileInfo } from "./classes"
import { removeDuplicatesFromString, isAlphaOrDigit } from "./utils"

export function removeNonDeterminism (vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): AutomataVertex[] {

    let vertexesDA: AutomataVertex[] = []

    for (let i: number = 0; i < grammarInfo.variablesSubsets.length; i++)
        vertexesDA.push(new AutomataVertex(grammarInfo.variablesSubsets[i],[], false))

    for (let i: number = 0; i < grammarInfo.terminals.length; i++) {

        for (let j: number = 0; j < grammarInfo.variablesSubsets.length; j++) {

            let edgeAux: string = grammarInfo.terminals[i]
            let isFinalState: boolean = false

            for (let k: number = 0; k < grammarInfo.variablesSubsets[j].length; k++) {

                let vertexIndex: number = grammarInfo.variables.indexOf(grammarInfo.variablesSubsets[j][k])

                if (vertexesNDA[vertexIndex].isFinalState)
                    isFinalState = true
    
                for (let l: number = 0; l < vertexesNDA[vertexIndex].edges.length; l++) {
                    
                    if (vertexesNDA[vertexIndex].edges[l][0] === grammarInfo.terminals[i]) 
                    {
                        if (vertexesNDA[vertexIndex].edges[l].length === 1) {
                            vertexesDA[j].edges.push(grammarInfo.terminals[i])
                            vertexesDA[j].isFinalState = isFinalState                  
                        }                        
                        
                        else if (isAlphaOrDigit(vertexesNDA[vertexIndex].edges[l][1])) {
                            edgeAux += vertexesNDA[vertexIndex].edges[l][1]
                        }
                    }
                }
            } 

            if (edgeAux.length > 1) {
                edgeAux = removeDuplicatesFromString(edgeAux)
                vertexesDA[j].edges.push(edgeAux)
                vertexesDA[j].isFinalState = isFinalState
            } 
        }
    }

    return vertexesDA
}