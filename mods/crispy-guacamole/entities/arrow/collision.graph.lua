script {
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
}