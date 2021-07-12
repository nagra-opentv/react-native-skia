import * as React from 'react';
import { AppRegistry, Text } from 'react-native';

const SimpleViewApp = React.Node = () => {
  return (
    <>
      <Text style={{ margin: 10 }}>DEFAULT FONT WEIGHT</Text>
      <Text style={{ margin: 10, fontWeight: 'normal' }}>FONT WEIGHT - NORMAL</Text>
      <Text style={{ margin: 10, fontWeight: 'bold' }}>FONT WEIGHT - BOLD</Text>
      <Text style={{ margin: 10, fontWeight: '100' }}>FONT WEIGHT - 100</Text>
    </>
  );
};

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
