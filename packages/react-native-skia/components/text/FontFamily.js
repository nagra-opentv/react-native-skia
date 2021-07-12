import * as React from 'react';
import { AppRegistry, Text } from 'react-native';

const SimpleViewApp = React.Node = () => {
  return (
    <>
      <Text style={{ margin: 10 }}>DEFAULT FONT FAMILY</Text>
      <Text style={{ margin: 10, fontFamily: "Times New Roman" }}>FONT FAMILY - Times New Roman</Text>
      <Text style={{ margin: 10, fontFamily: "Ariel" }}>FONT FAMILY - Ariel</Text>
    </>
  );
};

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
