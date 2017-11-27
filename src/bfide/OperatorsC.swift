//
//  OperatorsC.swift
//  BF_IDE
//
//  Created by Lucas Menezes on 02/07/16.
//  Copyright © 2016 MWAPPTECH. All rights reserved.
//

import Foundation


//From http://stackoverflow.com/questions/35158422/the-and-operators-have-been-deprecated-xcode-7-3

prefix operator ++
postfix operator ++

prefix operator --
postfix operator --


// Increment
prefix func ++( x: inout Int) -> Int {
    x += 1
    return x
}

postfix func ++( x: inout Int) -> Int {
    x += 1
    return (x - 1)
}

prefix func ++( x: inout UInt) -> UInt {
    x += 1
    return x
}

postfix func ++( x: inout UInt) -> UInt {
    x += 1
    return (x - 1)
}

prefix func ++( x: inout Int8) -> Int8 {
    x += 1
    return x
}

postfix func ++(x: inout Int8) -> Int8 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout UInt8) -> UInt8 {
    x += 1
    return x
}

postfix func ++(x: inout UInt8) -> UInt8 {
    x += 1
    return (x - 1)
}
prefix func ++(x: inout Int16) -> Int16 {
    x += 1
    return x
}

postfix func ++(x: inout Int16) -> Int16 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout UInt16) -> UInt16 {
    x += 1
    return x
}

postfix func ++(x: inout UInt16) -> UInt16 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout Int32) -> Int32 {
    x += 1
    return x
}

postfix func ++(x: inout Int32) -> Int32 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout UInt32) -> UInt32 {
    x += 1
    return x
}

postfix func ++(x: inout UInt32) -> UInt32 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout Int64) -> Int64 {
    x += 1
    return x
}

postfix func ++(x: inout Int64) -> Int64 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout UInt64) -> UInt64 {
    x += 1
    return x
}

postfix func ++(x: inout UInt64) -> UInt64 {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout Double) -> Double {
    x += 1
    return x
}

postfix func ++(x: inout Double) -> Double {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout Float) -> Float {
    x += 1
    return x
}

postfix func ++(x: inout Float) -> Float {
    x += 1
    return (x - 1)
}

prefix func ++(x: inout Float80) -> Float80 {
    x += 1
    return x
}

postfix func ++(x: inout Float80) -> Float80 {
    x += 1
    return (x - 1)
}

//prefix func ++<T : _Incrementable>( i: inout T) -> T {
//    i = i.successor()
//    return i
//}
//
//postfix func ++<T : _Incrementable>( i: inout T) -> T {
//    let y = i
//    i = i.successor()
//    return y
//}

// Decrement
prefix func --(x: inout Int) -> Int {
    x -= 1
    return x
}

postfix func --(x: inout Int) -> Int {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout UInt) -> UInt {
    x -= 1
    return x
}

postfix func --(x: inout UInt) -> UInt {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Int8) -> Int8 {
    x -= 1
    return x
}

postfix func --(x: inout Int8) -> Int8 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout UInt8) -> UInt8 {
    x -= 1
    return x
}

postfix func --(x: inout UInt8) -> UInt8 {
    x -= 1
    return (x + 1)
}
prefix func --(x: inout Int16) -> Int16 {
    x -= 1
    return x
}

postfix func --(x: inout Int16) -> Int16 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout UInt16) -> UInt16 {
    x -= 1
    return x
}

postfix func --(x: inout UInt16) -> UInt16 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Int32) -> Int32 {
    x -= 1
    return x
}

postfix func --(x: inout Int32) -> Int32 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout UInt32) -> UInt32 {
    x -= 1
    return x
}

postfix func --(x: inout UInt32) -> UInt32 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Int64) -> Int64 {
    x -= 1
    return x
}

postfix func --(x: inout Int64) -> Int64 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout UInt64) -> UInt64 {
    x -= 1
    return x
}

postfix func --(x: inout UInt64) -> UInt64 {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Double) -> Double {
    x -= 1
    return x
}

postfix func --(x: inout Double) -> Double {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Float) -> Float {
    x -= 1
    return x
}

postfix func --(x: inout Float) -> Float {
    x -= 1
    return (x + 1)
}

prefix func --(x: inout Float80) -> Float80 {
    x -= 1
    return x
}

postfix func --(x: inout Float80) -> Float80 {
    x -= 1
    return (x + 1)
}

//prefix func --<T : BidirectionalIndexType>(inout i: T) -> T {
//    i = i.predecessor()
//    return i
//}
//
//postfix func --<T : BidirectionalIndexType>(inout i: T) -> T {
//    let y = i
//    i = i.predecessor()
//    return y
//}

