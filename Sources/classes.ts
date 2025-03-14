export class InputFileInfo {
    grammarName: string = ""
    variables: string[] = []
    terminals: string[] = []
    productionsName: string = ""
    initialVariable: string = ""
    productionRules: string[] = []
    variablesSubsets: string[] = []
    acceptsEmptyWord: boolean = false
}

export class AutomataVertex {
    variable: string = ""
    edges: string[] = []
    isFinalState: boolean = false
    constructor (variable: string, edges: string[], isFinalState: boolean) {
        this.variable = variable
        this.edges = edges
        this.isFinalState = this.isFinalState
    }
}