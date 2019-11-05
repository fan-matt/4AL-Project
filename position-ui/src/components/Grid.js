import React from 'react';

import { Stage , Layer , Line } from 'react-konva';


export default class Grid extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            scale: 1 ,
        }
    }

    createIntArray = (start , stop) => {
        let array = new Array(stop - start + 1);

        for(let i = 0; i < stop - start + 1; i ++) {
            array[i] = start + i;
        }

        return array;
    }

    renderGrid = () => {
        const BUFFER = 10;
        const NUM_GRIDLINES = 10;
        const HEIGHT = this.props.height - BUFFER * 2;
        const WIDTH = this.props.width - BUFFER * 2;

        let xLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let yCoordinate = HEIGHT / (2 * NUM_GRIDLINES) * num + this.props.height / 2;
            return <Line points={[0 , yCoordinate , this.props.width , yCoordinate]} stroke='grey' />
        });

        let yLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let xCoordinate = WIDTH / (2 * NUM_GRIDLINES) * num + this.props.width / 2;
            return <Line points={[xCoordinate , 0 , xCoordinate , this.props.height]} stroke='grey' />
        });

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