// gryphon output: main.kt
//  main.swift
//  bfide-bfi
//
//  Created by Lucas Menezes on 27/11/17.
//  Copyright © 2017 MWAPPTECH. All rights reserved.
//

import Foundation
//dump(CommandLine.arguments)

func readFromFile(path : String) throws -> String
{
    let url = URL(fileURLWithPath: path)
    let myString = try String(contentsOf: url)
    return myString
}

let args = CommandLine.arguments
if(args.count >= 3){
    
}
else if(args.count == 2){
    
    do {
        let program = try readFromFile(path: args[1]);
        print(program)  
        // let interpreter = BrainfuckInterpreter(cells: 30000)
        //     interpreter.dbg_messages = false
        //     interpreter.setProgram(program: program)
        //     interpreter.inputRedirect = false
        //     interpreter.outputRedirect = false
        //     interpreter.execute()
    } catch {
        print("Couldn't read file: \(args[1])");
        exit(1)
    }
    exit(0)
    
}
else{
    print("Error!\nProgram usage is: \n\(args[0]) sourcecode");
    exit(-1)
}
