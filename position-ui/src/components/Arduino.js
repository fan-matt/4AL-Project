import React from 'react';

import { Circle } from 'react-konva';

export default class Arduino extends React.Component {
    render() {
        return(
            <>
                <Circle 
                    x={this.props.xPos} 
                    y={this.props.yPos}
                    radius={20 * this.props.scale * 3}
                    fill="red"
                />
            </>
        );
    }
}