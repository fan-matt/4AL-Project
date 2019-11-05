import React from 'react';

import Grid from './Grid';


class App extends React.Component {
    constructor(props) {
        super(props);

        this.state = {
            width: window.innerWidth ,
            height: window.innerHeight ,
        };
    }

    // React Lifecycle Methods //
    componentDidMount() {
        window.addEventListener('resize' , this.resize)
    }

    componentWillUnmount() {
        window.removeEventListener('resize');
    }
    // End React Lifecycle Methods //

    resize = () => {
        this.setState({
            width: window.innerWidth ,
            height: window.innerHeight ,
        });
    }

    render() {
        return (
            <div className="App">
                <Grid width={this.state.width} height={this.state.height} />
            </div>
        );
    }
}

export default App;
