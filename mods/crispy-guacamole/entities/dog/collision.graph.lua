script {
  links = {
    {
      1,
      1,
      6,
      1,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      3,
      1,
      1,
      2,
    },
    {
      4,
      1,
      1,
      3,
    },
    {
      5,
      1,
      1,
      4,
    },
  },
  nodes = {
    {
      name = 'components/collisioncomponent',
    },
    {
      loadArguments = {
        'cylinder',
      },
      name = 'string',
    },
    {
      loadArguments = {
        0.15000000596046,
      },
      name = 'number',
    },
    {
      loadArguments = {
        0.69999998807907,
      },
      name = 'number',
    },
    {
      loadArguments = {
        true,
      },
      name = 'boolean',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
  },
}