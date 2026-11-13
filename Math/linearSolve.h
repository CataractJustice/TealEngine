#pragma once
#include <limits>
//Returns x coordinate of an intersection point of a line going through points (px0, py0) and (px1, py1) and a line defined as y=find 
template<class T>
inline T solveLinearEquationFromTwoPoints(T px0, T py0, T px1, T py1, T find) 
{
    return (find - py0) / ((py1 - py0) / (px1 - px0)) + px0;
}
// Calculates the solution for a linear equation of the form y = mx + b, where:
//   y is the dependent variable,
//   b is the y-intercept (the point where the line intersects the y-axis),
//   m is the slope of the line (the rate at which y changes with respect to x).
// Returns the value of x for a given y using the formula: x = (y - b) / m.
template<class T>
inline T solveLinearEquationFromHeightAndSlope(T b, T m, T y) 
{
	return ((y - b)/(m));
}