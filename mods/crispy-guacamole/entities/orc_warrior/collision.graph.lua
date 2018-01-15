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
        0.25,
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
        true,
      },
      name = 'bool',
    },
  },
}