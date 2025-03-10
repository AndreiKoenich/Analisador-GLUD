export class InputFileInfo {
    grammarName: string = ""
    variables: string[] = []
    terminals: string[] = []
    productionsName: string = ""
    initialVariable: string = ""
    productionRules: string[] = []
}

export class AutomataVertex {
    variable: string = ""
    edges: string[] = []
    neighbors: AutomataVertex[] = []
}