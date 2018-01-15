script {
  nodes = {
    {
      name = 'components/lifecomponent',
    },
    {
      name = 'number',
      loadArguments = {
        200,
      },
    },
    {
      name = 'script/scriptoutput',
      loadArguments = {
        {
          5,
          'Table',
        },
      },
    },
    {
      name = 'entity/playanimation',
    },
    {
      name = 'string',
      loadArguments = {
        'death',
      },
    },
    {
      name = 'number',
      loadArguments = {
        1,
      },
    },
    {
      name = 'bool',
      loadArguments = {
        true,
      },
    },
  },
  links = {
    {
      1,
      1,
      3,
      1,
    },
    {
      1,
      3,
      4,
      1,
    },
    {
      1,
      4,
      4,
      2,
    },
    {
      2,
      1,
      1,
      1,
    },
    {
      5,
      1,
      4,
      3,
    },
    {
      6,
      1,
      4,
      4,
    },
    {
      7,
      1,
      4,
      5,
    },
  },
}