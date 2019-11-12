import React from 'react';
import ReactDOM from 'react-dom';

import Arduino from './Arduino';

import { Stage , Layer , Line } from 'react-konva';


export default class Grid extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            buffer: 10 ,
            scale: 1 ,
            defaultLines: 10 ,
            xPos: 0 ,
            yPos: 0 ,
        }
    }

    // React Lifecycle Methods //
    componentDidMount() {
        let thisElement = ReactDOM.findDOMNode(this);
        thisElement.addEventListener('wheel' , this.scrollScale);
    }

    componentWillUnmount() {
        let thisElement = ReactDOM.findDOMNode(this);
        thisElement.removeEventListener('wheel' , this.scrollScale);
    }
    // End React Lifecycle Methods //

    scrollScale = (event) => {
        event.preventDefault();

        let sign = (event.deltaY < 0) ? -1 : 1;
        let newScale = Math.round(this.state.scale + sign);

        if(newScale < 1) {
            return;
        }

        this.setState({
            scale: newScale
        });
    }
    
    createIntArray = (start , stop) => {
        let array = new Array(stop - start + 1);

        for(let i = 0; i < stop - start + 1; i ++) {
            array[i] = start + i;
        }

        return array;
    }

    renderGrid = () => {
        const NUM_GRIDLINES = this.state.scale * this.state.defaultLines;
        const HEIGHT = this.props.height - this.state.buffer * 2;
        const WIDTH = this.props.width - this.state.buffer * 2;
        const stepSize = Math.max(HEIGHT , WIDTH) / (2 * NUM_GRIDLINES);
        
        let xLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let yCoordinate = stepSize * num + this.props.height / 2;
            return <Line points={[0 , yCoordinate , this.props.width , yCoordinate]} stroke='grey' />
        });

        let yLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let xCoordinate = stepSize * num + this.props.width / 2;
            return <Line points={[xCoordinate , 0 , xCoordinate , this.props.height]} stroke='grey' />
        });


        let arduinoX = this.state.xPos * stepSize + this.props.width / 2;
        let arduinoY = -this.state.yPos * stepSize + this.props.height / 2;


        return(
            <>
                {/* Generate lines */}
                {xLines}
                {yLines}

                {/* Coordinate axes */}
                <Line points={[this.props.width / 2 , 0 , this.props.width / 2 , this.props.height]}
                    stroke='black' strokeWidth={5} />

                <Line points={[0 , this.props.height / 2 , this.props.width , this.props.height / 2]}
                    stroke='black' strokeWidth={5} />

                {/* Draw Arduino at correct position */}
                <Arduino xPos={arduinoX} yPos={arduinoY} scale={1.0 / this.state.scale} />
            </>
        );   
    }

    render() {
        return(
            <Stage width={this.props.width} height={this.props.height}>
                <Layer>
                    {this.renderGrid()}
                </Layer>
            </Stage>
        );
    }
}