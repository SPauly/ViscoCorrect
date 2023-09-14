# ViscoCorrect [![FOSSA Status](https://app.fossa.com/api/projects/git%2Bgithub.com%2FSPauly%2FViscoCorrect.svg?type=shield&issueType=license)](https://app.fossa.com/projects/git%2Bgithub.com%2FSPauly%2FViscoCorrect?ref=badge_shield) 
**ViscoCorrect** is a tool that calculates correction factors for centrifugal pumps for the viscosity, head and flowrate of a pumped liquid based on its viscosity.
The tool uses a graphical approach based on [this graph](https://www.researchgate.net/figure/The-graph-obtained-by-the-American-Institute-of-hydraulics_fig1_335209726) obtained by the American Institute of Hydraulics.


**Disclaimer:** This software is purely experimental and all calculated values should be verified manually. Use at your own risk. Note that the used standard is **deprecated!** The HI advises only using the latest [ANSI/HI 9.6.7 Standard](https://www.pumps.org/what-we-do/standards/?pumps-search-product=9.6.7). 

***

![ViscoCorrect_dark](https://github.com/SPauly/ViscoCorrect/assets/43349007/3e806dcc-51a2-4819-a6df-ee711968ee51)

## What's been implemented so far
ViscoCorrect is still in its early stages of development but the major features have already been implemented! Note however that they still require some testing.
- Calculate the correction factors for `eta`, `H` and `Q`
- Accept and convert different input units:
    - Flowrate: m^3/h, l/min, GPM
    - Total differential head: m, ft
    - Kinematic viscosity: mm^2/h, cSt, cP, mPas
- Display the graph and create custom functions based on the input [beta]
- Feedback function that lets the user provide the manually calculated values for future improvements 
- Dark and light mode
- Implementation of an open workspace that lets the user freely use a dockspace [beta]

## Usage
Windows users can download the latest binaries in the [Release](https://github.com/SPauly/ViscoCorrect/releases) section.

Mac and Linux are not supported at the moment. However, it should be fairly easy to  do so in the future since the code base was designed with that in mind.

### Providing feedback
I am a computer science student and not an engineer. Although I tried my best to ensure a close match between the approximated functions and those from the graph, there may still be inaccuracies. 

I therefore created a button in ViscoCorrect that says `provide feedback`. The popup window lets you enter the manually calculated correction factors for the latest calculation. This data then is saved in a `feedback.csv` file.

It would be extremely helpful to me and the development of this project if you could upload this file [here!](https://github.com/SPauly/ViscoCorrect/issues/41) Thank you in advance!

Also, do not hesitate to create a new issue and provide your feedback regarding the software :)

## Developers
If you want to build ViscoCorrect by yourself you can simply use the cmake build system already in place. The currently supported and tested platform is MSVC.
Proper code documentation is already in progress. 
