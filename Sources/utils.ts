import { alphaOrDigitRegex } from "./constants";
import { InputFileInfo } from "./classes";

export function getSubsets(stringArray: string[]): string[] {
    let subsets: string[] = [];
    let arraySize:number = stringArray.length;
    
    for (let i: number = 1; i < (1 << arraySize); i++) {
        let subset: string = "";
        for (let j:number = 0; j < arraySize; j++) {
            if (i & (1 << j)) {
                subset += stringArray[j];
            }
        }
        subsets.push(subset.split("").sort().join(""))
    }
        
    return subsets;
}

export function removeDuplicatesFromString(str: string): string {
    return [...new Set(str)].join('')
}

export function removeDuplicatesFromStringArray(str: string[]): string[] {
    return [...new Set(str)]
}

export function hasDuplicateElements (stringArray: string[]) : boolean {
    return new Set(stringArray).size != stringArray.length 
}

export function isAlphaOrDigit (character: string) : boolean {
    return (alphaOrDigitRegex.test(character))
}

export function testStringArrayIntersection(a: string[], b: string[]) : boolean {
    const s = new Set(b)
    return [...new Set(a)].some((x) => s.has(x))
}

export function printGrammarInfo (grammarInfo: InputFileInfo) : void {
    console.log(grammarInfo.grammarName)
    console.log(grammarInfo.initialVariable)
    console.log(grammarInfo.variables)
    console.log(grammarInfo.terminals)
    console.log(grammarInfo.productionsName)
    console.log(grammarInfo.productionRules)
}
