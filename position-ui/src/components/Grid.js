import React from 'react';

import { Stage , Layer , Line } from 'react-konva';


export default class Grid extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            scale: 1 ,
        }
    }

    renderGrid = () => {
        return(
            <>
                {/* Coordinate axes */}
                <Line points={[this.props.width / 2 , 0 , this.props.width / 2 , this.props.height]}
                    stroke='black' />

                <Line points={[0 , this.props.height / 2 , this.props.width , this.props.height / 2]}
                    stroke='black' />


                {/* Generate rest of the lines */}
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