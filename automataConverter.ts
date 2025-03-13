import { AutomataVertex, InputFileInfo } from "./classes"
import { isAlphaOrDigit } from "./fileDataController"

export function removeNonDeterminism (vertexesND: AutomataVertex[], grammarInfo: InputFileInfo): AutomataVertex[] {

    let vertexesD: AutomataVertex[] = []
    const newVariables: string[] = getSubsets(grammarInfo.variables)

    for (let i: number = 0; i < newVariables.length; i++)
        vertexesD.push(new AutomataVertex(newVariables[i],[], false))

    for (let i: number = 0; i < grammarInfo.terminals.length; i++) {

        for (let j: number = 0; j < newVariables.length; j++) {

            let edgeAux: string = grammarInfo.terminals[i]
            let containsTerminal: boolean = false
            let isFinalState: boolean = false

            for (let k: number = 0; k < newVariables[j].length; k++) {

                let vertexIndex: number = grammarInfo.variables.indexOf(newVariables[j][k])

                if (vertexesND[vertexIndex].isFinalState)
                    isFinalState = true

                if (vertexesND[vertexIndex].edges.join("").includes(grammarInfo.terminals[i]))
                    containsTerminal = true
    
                for (let l: number = 0; l < vertexesND[vertexIndex].edges.length; l++) {

                    if (vertexesND[vertexIndex].edges[l][0] == grammarInfo.terminals[i] 
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

function getSubsets(stringArray: string[]): string[] {
    let subsets: string[] = [];
    let arraySize:number = stringArray.length;
    
    for (let i: number = 1; i < (1 << arraySize); i++) {
        let subset: string = "";
        for (let j:number = 0; j < arraySize; j++) {
            if (i & (1 << j)) {
                subset += stringArray[j];
            }
        }
        subsets.push(subset);
    }
    
    return subsets;
}

function removeDuplicates(str: string): string {
    return [...new Set(str)].join('');
}