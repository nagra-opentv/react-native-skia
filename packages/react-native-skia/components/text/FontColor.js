import * as React from 'react';
import { AppRegistry, Text } from 'react-native';

const SimpleViewApp = React.Node = () => {
  return (
    <>
      <Text style={{ margin: 10 }}>DEFAULT FONT COLOR</Text>
      <Text style={{ margin: 10, color: 'red' }}>FONT COLOR - RED</Text>
      <Text style={{ margin: 10, color: 'blue' }}>FONT COLOR - BLUE</Text>
    </>
  );
};

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
