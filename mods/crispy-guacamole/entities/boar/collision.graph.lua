script {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      3,
      1,
      1,
      1,
    },
    {
      4,
      1,
      1,
      2,
    },
    {
      5,
      1,
      1,
      3,
    },
    {
      6,
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
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        'cylinder',
      },
      name = 'string',
    },
    {
      loadArguments = {
        0.20000000298023,
      },
      name = 'number',
    },
    {
      loadArguments = {
        1,
      },
      name = 'number',
    },
    {
      loadArguments = {
        false,
      },
      name = 'bool',
    },
  },
}