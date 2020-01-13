//
//  BrainfuckInterpreter.swift
//  Brainfuck_IDE
//
//  Created by Lucas Menezes on 07/03/16.
//  Copyright © 2016 MWAPPTECH. All rights reserved.
//

import Cocoa
import Foundation


class BrainfuckInterpreter {
    private (set) var program : [CChar] = [CChar]() //C string
    private (set) var memory : [Int8]! = [Int8]() //8 bit
    private var mem_idx : Int // equivalent to pointer to memory
    private var prg_idx : Int // equivalent to pointer to string
    private var loopStack : [Int] = [Int]()
    private (set) var output : [CChar] = [CChar]() //stdout equiv
    private (set) var input : [CChar] = [CChar]() //stdin equiv
    private (set) var output_s : String = String()
    private (set) var input_s : String = String()
    
    var dbg_messages = true
    var inputRedirect : Bool!
    var outputRedirect : Bool!
    
    // signal to stop interpreter
    private var sigStop : Bool!
    
    // tells user application that interpreter is waiting
    private (set) var waitingInput : Bool!
    
    // tells user application that interpreter has finished
    private (set) var finished : Bool!
    
    private (set) var crashed : Bool!
    
    private (set) var breakpointEnabled : Bool!
    
    private var codeWhenOutput : ((_ str : String) -> ())!
    private var codeWhenInput : (() -> ())!
    
    let instructions  = [62 : BrainfuckInterpreter.greater, //62 >
        60 : BrainfuckInterpreter.lesser ,//60 <
        43 : BrainfuckInterpreter.plus , //43 +
        45 : BrainfuckInterpreter.minus , //45 -
        46 : BrainfuckInterpreter.dot ,//46 .
        44 : BrainfuckInterpreter.comma, //44 ,
        91 : BrainfuckInterpreter.bracketOpen, //91 [
        93 : BrainfuckInterpreter.bracketClose] //93 ]
    
    private var benchmark = [Double]()
    private var longestTime : Double! = 0.0
    
    init(cells : Int)
    {
        self.mem_idx = 0
        self.prg_idx = 0
        memory = [Int8](repeating: 0, count: cells) //zeroed memory
        self.inputRedirect = true
        self.outputRedirect = true
        self.sigStop = false
        self.waitingInput = false
        self.crashed = false
        self.breakpointEnabled = false
        //self.testEachFuncTime()
    }
    
    /**
     Sets Program String to the interpreter.
     This program will run with execute()
     
     - parameter program : String. A Swift String with the program instructions.
     The program must consist of a valid brainfuck program.
     - attention: Must not have brainfuck instructions inside comments
     - returns: nothing
     */
    func setProgram(program : String)
    {
        dprint(program.count)
        self.program = program.cString(using: String.Encoding.utf8)!
        self.prg_idx = 0 //program must be read again
        dprint(self.program.count)
    }
    
    func setCodeWhenOutput(code : @escaping (_ str : String)->())
    {
        self.codeWhenOutput = code
    }

    //MARK: LoopStack
    private func loopPush(index : Int)
    {
        loopStack.append(index)
    }
    private func loopPop()
    {
        loopStack.removeLast()
    }
    private func loopTop() -> Int
    {
        return loopStack.last!
    }
    private func loopFree()
    {
        loopStack.removeAll()
    }
    //MARK: Instructions
    
    func greater()
    {
        self.mem_idx = self.mem_idx &+ 1 //enable overflow
        if(self.mem_idx >= self.memory.count)
        {
            self.stop()
            self.crashed = true
            let str = "Access violation, there is no cell \(self.mem_idx)"
            self.output_s += String(format: "\nbfi: ACCESS VIOLATION. THERE IS NO CELL NUMBER %d\n", self.mem_idx)
            print(str)
            return
        }
    }
    func lesser()
    {
        self.mem_idx = self.mem_idx &- 1 //enable overflow
        if(self.mem_idx < 0)
        {
            self.stop()
            self.crashed = true
            let str = "Access violation, there is no cell \(self.mem_idx)"
            self.output_s += String(format: "\nbfi: ACCESS VIOLATION. THERE IS NO CELL NUMBER %d\n", self.mem_idx)
            print(str)
            return
        }
    }
    func plus()
    {
        memory[mem_idx] = memory[mem_idx] &+ 1 //enable overflow
    }
    func minus()
    {
        memory[mem_idx] = memory[mem_idx] &- 1 //enable overflow
    }
    func dot()
    {
        if(self.outputRedirect == true)
        {
            self.output.append(memory[mem_idx])
            //self.output_s.append(c: UnicodeScalar(UInt8(bitPattern: memory[mem_idx])))
            let i8 = memory[mem_idx]
            let ui8 = UInt8(bitPattern: i8)
            let scalar =  UnicodeScalar(ui8)
            let char = Character(scalar)
            self.output_s.append(char)
            
            //codeWhenOutput(str: cCharToString(memory[mem_idx]))
        }
        else
        {
            putchar(Int32(memory[mem_idx]))
        }
        //this print is a performance killer. Be Warned
        //print(String(UnicodeScalar(UInt8(bitPattern:  memory[mem_idx]))),terminator: "")
    }
    func comma()
    {
        //let keyboard = FileHandle.standardInput
        //let inputData = keyboard.availableData
        //let str = String(data: inputData, encoding:String.Encoding.utf8)
        //memory[mem_idx] = str?.cStringUsingEncoding(NSUTF8StringEncoding)!.first!.value?
        
        if(self.inputRedirect == true)
        {
            
            if(self.input.count <= 0 )
            {
                objc_sync_enter(self)
                dprint("bfi: INTERPRETER ASKS FOR INPUT")
                self.waitingInput = true
                objc_sync_exit(self)
            }
            while(self.input.count <= 0)
            {
                if(self.sigStop == true) { return }
                usleep(3000) // make thread wait for it
            }
            objc_sync_enter(self)
            self.waitingInput = false
            memory[mem_idx] = self.input[0]
            self.input.removeFirst()
            //print(self.input)
            objc_sync_exit(self)
        }
        else
        {
            memory[mem_idx] = Int8(getchar())
        }
    }
    func bracketOpen()
    {
        if(memory[mem_idx] != 0)
        {
            loopPush(index: prg_idx)
        }
        else
        {
            // skip while sequence
            //self.evaluateTime({
            var internalLoopCount = 0
            self.prg_idx += 1
            var s = self.program[self.prg_idx]
            while(!(s == 93 && internalLoopCount == 0)) // while not match stop requirements
            {
                if(s == 91)
                {
                    internalLoopCount += 1;
                }
                else if(s == 93)
                {
                    internalLoopCount -= 1;
                }
                self.prg_idx += 1
                s = self.program[self.prg_idx]
                //print("\(s) \(internalLoopCount) \(prg_idx)")
                //usleep(100)
            }
            //}, description: "Skip While")
        }
    }
    func bracketClose()
    {
        if(memory[mem_idx] != 0)
        {
            prg_idx = loopTop()
        }
        else
        {
            loopPop() // out of this loop
        }
    }
    
    
    //MARK: Execute
    //    func executeInstruction(instruction : String)
    //    {
    //        let char =  instruction.cString(using: String.Encoding.utf8)?.first! as! Int
    //        self.instructions[0]()
    //    }
    func executeStep()
    {
        
        let current = program[prg_idx]
        switch current
        {
        case Int8(62) :
            greater() //62 >
        case Int8(60) :
            lesser()//60 <
        case Int8(43) :
            plus() //43 +
        case Int8(45) :
            minus() //45 -
        case Int8(46) :
            dot() //46 .
        case Int8(44) :
            comma() //44 ,
        case Int8(91) :
            bracketOpen() //91 [
        case Int8(93) :
            bracketClose() //93 ]
        default:
            ()
        }
        prg_idx += 1
        
    }
    func execute()
    {
        self.finished = false
        dprint("Execute")
        self.evaluateTime(f: {
            while(self.prg_idx < self.program.count)
            {
                if(self.sigStop == true) { self.stopSequence(); return } // abort execution
                self.executeStep()
            }
            dprint("longestTime was \(self.longestTime!)")
        }, description: "execution")
        self.finished = true
        
    }
    func stop()
    {
        self.sigStop = true
    }
    /**
     Sends Input to the Interpreter.
     Thread safe.
     */
    func sendInput(inputStr : String )
    {
        objc_sync_enter(self)
        self.input_s += inputStr
        let input = inputStr.cString(using: String.Encoding.utf8)
        for c in input!
        {
            self.input.append(c)
        }
        //self.input.append(CChar(-1)) //EOF
        dprint("input received \(inputStr)")
        objc_sync_exit(self)
    }
    private func stopSequence()
    {
        dprint("bfi: EXECUTION ABORTED");
        self.finished = true
        return
    }
    
    //MARK: Auxiliary
    private class func cCharToString(char : CChar) -> String
    {
        return String(UnicodeScalar(UInt8(bitPattern: char)))
    }
    private func evaluateTime(f : ()->() , description : String)
    {
        dprint(description)
        let date1 = CFAbsoluteTimeGetCurrent()
        f()
        let date2 = CFAbsoluteTimeGetCurrent()
        let time = date2-date1
        self.longestTime =  time > longestTime ? time : longestTime
        dprint("it took \(time)")
    }
    private func testEachFuncTime()
    {
        evaluateTime(f: {self.greater()},description: "greater")
        evaluateTime(f: {self.lesser()},description: "lesser")
        evaluateTime(f: {self.plus()},description: "plus")
        evaluateTime(f: {self.minus()},description: "minus")
        evaluateTime(f: {self.dot()},description: "dot")
        //evaluateTime(f: {sendInput(inputStr: "a"); self.comma()},description: "comma")
        evaluateTime(f: {self.bracketOpen()}, description: "[")
        evaluateTime(f: {self.bracketClose()}, description: "]")
    }
    
    private func dprint( _ a : Any){
        if(dbg_messages){
            print(a)
        }
    }
    
    
}

