import { AutomataVertex, InputFileInfo } from "./classes"
import { emptyLanguageCode, finiteLanguageCode, infiniteLanguageCode } from "./constants"
import { isAlphaOrDigit, removeDuplicatesFromStringArray } from "./utils"

export function checkLanguageFinitude (vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): void {

    let finitudeCode: number = testFinitude (vertexesNDA, grammarInfo)

    switch (finitudeCode) {
        case emptyLanguageCode:
            console.log("\nA GLUD inserida corresponde a uma linguagem regular VAZIA.")
        break;
        case finiteLanguageCode:
            console.log("\nA GLUD inserida corresponde a uma linguagem regular FINITA.")
        break;
        case infiniteLanguageCode:
            console.log("\nA GLUD inserida corresponde a uma linguagem regular INFINITA.")
        break;
    }
}

function testFinitude (vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): number {

    let vertexIndex: number = grammarInfo.variables.indexOf(grammarInfo.initialVariable)

    if (vertexesNDA[vertexIndex].edges.length === 0 || !checkForFinalStates(grammarInfo))
        return emptyLanguageCode

    let reachableFromStartStates: string[] = findReachableStates(grammarInfo.initialVariable,vertexesNDA,grammarInfo)
    let reachableStatesFromVertex: string[] = []

    for (let i: number = 0; i < reachableFromStartStates.length; i++) {

        if (checkForLoop(reachableFromStartStates[i], vertexesNDA, grammarInfo) && checkFinalStateReachable(reachableFromStartStates[i], vertexesNDA, grammarInfo))
            return infiniteLanguageCode

        reachableStatesFromVertex = findReachableStates(reachableFromStartStates[i], vertexesNDA, grammarInfo)

        for (let j: number = 0; j < reachableStatesFromVertex.length; j++) {

            vertexIndex = grammarInfo.variables.indexOf(reachableStatesFromVertex[j])

            for (let k: number = 0; k < vertexesNDA[vertexIndex].edges.length; k++) 
                if (vertexesNDA[vertexIndex].edges[k].slice(1) === reachableFromStartStates[i])
                    return infiniteLanguageCode
        }
    }

    return finiteLanguageCode
}

function findReachableStates (stateName: string, vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): string[] {

    let reachableStates: string[] = []
    reachableStates.push(stateName)

    let vertexIndex: number = grammarInfo.variables.indexOf(grammarInfo.initialVariable)

    for (let i: number = 0; i < reachableStates.length; i++) {

        vertexIndex = grammarInfo.variables.indexOf(reachableStates[i])
        
        for (let j: number = 0; j < vertexesNDA[vertexIndex].edges.length; j++) {
            
            if (vertexesNDA[vertexIndex].edges[j].length > 1 && isAlphaOrDigit(vertexesNDA[vertexIndex].edges[j][1])) {
                reachableStates.push(vertexesNDA[vertexIndex].edges[j].slice(1))
                reachableStates = removeDuplicatesFromStringArray(reachableStates)
            }

        }
    }

    return reachableStates.slice(1)
}

function checkFinalStateReachable (stateName: string, vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): boolean {

    let vertexIndex: number = grammarInfo.variables.indexOf(stateName)
    for (let i: number = 0; i < vertexesNDA[vertexIndex].edges.length; i++) 
        if (vertexesNDA[vertexIndex].edges[i].length === 1)
            return true

    let reachableStatesFromVertex: string[] = findReachableStates(stateName, vertexesNDA, grammarInfo)
    for (let i: number = 0; i < reachableStatesFromVertex.length; i++) {

        let vertexIndex: number = grammarInfo.variables.indexOf(reachableStatesFromVertex[i])

        if(vertexesNDA[vertexIndex].isFinalState)
            return true

        for (let j: number = 0; j < vertexesNDA[vertexIndex].edges.length; j++) 
            if (vertexesNDA[vertexIndex].edges[j].length === 1)
                return true
    }

    return false
}

function checkForLoop (stateName: string, vertexesNDA: AutomataVertex[], grammarInfo: InputFileInfo): boolean {

    const vertexIndex: number = grammarInfo.variables.indexOf(stateName)
    for (let i: number = 0; i < vertexesNDA[vertexIndex].edges.length; i++) 
        if (vertexesNDA[vertexIndex].edges[i].slice(1) === stateName)
            return true
    return false
}

function checkForFinalStates (grammarInfo: InputFileInfo): boolean {

    for (let i: number = 0; i < grammarInfo.productionRules.length; i++) {
        if (grammarInfo.productionRules[i].replace(/[ \t]+/g, '').length <= 4)
            return true 
    }

    return false
}