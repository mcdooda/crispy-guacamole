script {
  links = {
    {
      1,
      1,
      5,
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
      4,
    },
  },
  nodes = {
    {
      name = 'components/collisioncomponent',
    },
    {
      loadArguments = {
        'sphere',
      },
      name = 'string',
    },
    {
      loadArguments = {
        0.010099999606609,
      },
      name = 'number',
    },
    {
      loadArguments = {
        false,
      },
      name = 'bool',
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