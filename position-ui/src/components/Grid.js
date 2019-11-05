import React from 'react';
import ReactDOM from 'react-dom';

import { Stage , Layer , Line } from 'react-konva';


export default class Grid extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            scale: 1.3 ,
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

        let newScale = this.state.scale + event.deltaY * .1;

        if(newScale <= .1) {
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
        const BUFFER = 10;
        const NUM_GRIDLINES = Math.round(10 * this.state.scale);
        const HEIGHT = this.props.height - BUFFER * 2;
        const WIDTH = this.props.width - BUFFER * 2;
        const stepSize = Math.max(HEIGHT , WIDTH) / (2 * NUM_GRIDLINES);

        console.log("gridlines: " + NUM_GRIDLINES);
        
        let xLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let yCoordinate = stepSize * num + this.props.height / 2;
            return <Line points={[0 , yCoordinate , this.props.width , yCoordinate]} stroke='grey' />
        });

        let yLines = this.createIntArray(-NUM_GRIDLINES , NUM_GRIDLINES).map(num => {
            let xCoordinate = stepSize * num + this.props.width / 2;
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